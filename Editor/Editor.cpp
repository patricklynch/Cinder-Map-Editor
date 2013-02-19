#include "Editor.h"
#include "Camera.h"
#include "Input.h"
#include "Terrain.h"
#include "EditorConstants.h"
#include "AssetManager.h"

#include <algorithm>

#include "cinder/CinderMath.h"
#include "cinder/app/App.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

Editor::Editor( Game* game ) : mGame( game ), mTexturePaint( NULL ), mPanel( NULL ), mCamera( NULL ), mKeyCommands( NULL )
{
	mState.mode = MODE_PAINT_TEXTURE;
	
	mCamera = ly::Camera::get();
	mCamera->setFov( 50 );
	mCamera->setZoom( 30 );
	mCamera->setAngle( -45.0f);
	mCamera->rotation.y = 45.0f;
	
	mTexturePaint = new EditorTexturePaint( mState.texturePaint );
	
	mPanel = new EditorPanel( this );
	currentElevationTarget = kUndefinedElevation;
	mGame->mDelegate = this;
	mKeyCommands = new EditorKeyCommands( this );
	
	mDidPaintStraightLine = false;
	
	// Create EditorSeletion as a wrapper for each block in the game
	std::vector<Block*>& blocks = mGame->blocks();
	for( std::vector<Block*>::iterator iter = blocks.begin(); iter != blocks.end(); iter++ ) {
		mSelections.push_back( new EditorSelection( *iter, this ) );
	}
	// Set references to each block's surrounding blocks
	for( std::vector<EditorSelection*>::iterator iter = mSelections.begin(); iter != mSelections.end(); iter++) {
		(*iter)->findSurroundingBlocks( mSelections );
		(*iter)->mBlock->mTexturePaintMask = mTexturePaint->paintMask();
	}
}

Editor::~Editor()
{
	delete mTexturePaint;
	delete mKeyCommands;
}

void Editor::removeSelectionForBlock( Block* block )
{
	EditorSelection* selection = findBlock( mSelections, block->tilePosition );
	if ( selection ) {
		std::vector<EditorSelection*>::iterator match = std::find( mSelections.begin(), mSelections.end(), selection );
		if ( match != mSelections.end() ) {
			delete *match;
			mSelections.erase( match );
		}
	}
}

void Editor::mapCenterDidUpdate( ci::Vec3i center )
{
	for( std::vector<EditorSelection*>::iterator iter = mSelections.begin(); iter != mSelections.end(); iter++ ) {
		EditorSelection* selection = *iter;
		Vec3f resetPosition = Game::blockResetPosition( selection->getTilePosition(), center, kDefaultMaxVisibleTileRadius );
		selection->setTilePosition( resetPosition );
	}
}

Block* Editor::findBlock( std::vector<Block*>& blocks, ci::Vec3i tilePos )
{
	for( std::vector<Block*>::iterator iter = blocks.begin(); iter != blocks.end(); iter++ ) {
		if ( (*iter)->tilePosition == tilePos ) {
			return *iter;
		}
	}
	return NULL;
}

EditorSelection* Editor::findBlock( std::vector<EditorSelection*>& blocks, ci::Vec3i tilePos )
{
	for( std::vector<EditorSelection*>::iterator iter = blocks.begin(); iter != blocks.end(); iter++ ) {
		if ( (*iter)->getTilePosition() == tilePos ) {
			return *iter;
		}
	}
	return NULL;
}

void Editor::update( const float deltaTime )
{
	mEditorCamera.update( deltaTime );
	
	mKeyCommands->update( deltaTime );
	
	mTexturePaint->update( deltaTime );
	
	mPanel->update( deltaTime );
	
	applyUserEdits();
	
	SelectionMode_t selectionMode = SELECTION_NONE;
	
	if ( mState.mode == MODE_PAINT_ELEVATION ) {
		selectionMode = (mState.elevationRange % 2 == 0) ? SELECTION_POINT : SELECTION_FACE;
	}
	else if ( mState.mode == MODE_PAINT_TEXTURE ) {
		selectionMode = SELECTION_FACE;
	}
	
	// Update the selection objects
	std::vector<EditorSelection*>::iterator iter;
	for( iter = mSelections.begin(); iter != mSelections.end(); iter++) {
		(*iter)->setSelectionMode( selectionMode );
		(*iter)->update( deltaTime );
	}
	
	purgeBlocksWithNoMesh();
}

void Editor::purgeBlocksWithNoMesh()
{
	std::vector<EditorSelection*>::iterator iter;
	for( iter = mSelections.begin(); iter != mSelections.end(); iter++) {
		if ( (*iter)->mBlock->meshType() == BlockMeshNone ) {
			(*iter)->mBlock->setMeshType( BlockMeshCenter, 0.0f );
			setElevation( *iter, (*iter)->getTilePosition().y-1, (*iter)->mBlock->terrainIndex() );
			(*iter)->update( 0.0f );
		}
	}
}

void Editor::clearActiveSelections()
{
	std::vector<EditorSelection*>::iterator iter;
	for( iter = mSelections.begin(); iter != mSelections.end(); iter++) {
		(*iter)->unhighlight();
	}
	mActiveSelections.clear();
}

void Editor::applyUserEdits()
{
	if ( mState.mode == MODE_PAINT_ELEVATION ) {
		clearActiveSelections();
		Ray ray = mCamera->rayIntoScene( Input::get()->mousePosition() );
		mActiveSelections = select( ray, mState.elevationRange );
		std::vector<EditorSelection*>::iterator iter;
		for( iter = mActiveSelections.begin(); iter != mActiveSelections.end(); iter++) {
			(*iter)->highlight();
		}
	}
	else if ( mState.mode == MODE_PAINT_TEXTURE ) {
		Ray ray = mCamera->rayIntoScene( Input::get()->mousePosition() );
		std::vector<EditorSelection*> selections = select( ray, 1 );
		if ( selections.size() > 0 ) {
			float elevation = selections[0]->getTilePosition().y;
			float result;
			ray.calcPlaneIntersection( Vec3f( 0, elevation + 1, 0 ), Vec3f::yAxis(), &result );
			mTexturePaint->setTarget( ray.calcPosition( result ) );
		}
		else mTexturePaint->resetPosition();
		mTexturePaint->setState( mState.texturePaint );
	}
	
	Input* input = Input::get();
	MouseDrag& drag = input->getMouseDrag();
	if ( !drag.isActive && drag.mouseButton == MOUSE_LEFT ) {
		if ( mState.mode == MODE_PAINT_ELEVATION ) {
			// If the editing just stopped
			if ( mDragWasActive ) {
				for( std::vector<EditorSelection*>::iterator iter = mSelections.begin(); iter != mSelections.end(); iter++ ) {
					(*iter)->editingComplete();
				}
				currentElevationTarget = kUndefinedElevation;
				mDidPaintStraightLine = false;
			}
		}
		else if ( mState.mode == MODE_PAINT_TEXTURE ) {
			EditorCommandPaintTexture* cmd = dynamic_cast<EditorCommandPaintTexture*>( mCommandQueue.getCurrentCommand() );
			if ( cmd ) { cmd->commandComplete(); }
		}
	}
	else if ( drag.isActive && drag.mouseButton == MOUSE_LEFT ) {
		if ( mState.mode == MODE_PAINT_ELEVATION ) {
			
			// Find all selections between here and the current selection point and add to mActiveSelections
			if ( drag.isShiftDown && mLastSelectionTarget != kTargetUndefined ) {
				Vec3f origin = mLastSelectionTarget;
				Vec3f target = mActiveSelections[0]->boundingBoxCenter();
				selectStraightLine( origin, target, drag.isMetaDown );
			}
			
			if ( mActiveSelections.size() > 0 ) {
				if ( currentElevationTarget == kUndefinedElevation ) {
					currentElevationTarget = mState.elevationHeight;
				}
				
				EditorCommandModifyElevation* cmd = new EditorCommandModifyElevation();
				cmd->activeSelections = mActiveSelections;
				cmd->editor = this;
				cmd->amount = currentElevationTarget;
				cmd->terrainIndex = mState.elevationSelection;
				mCommandQueue.addCommand( cmd );
			}
			
			if (mActiveSelections.size() > 0 ) {
				mLastSelectionTarget = mActiveSelections[0]->boundingBoxCenter();
			} else {
				mLastSelectionTarget = kTargetUndefined;
			}
		}
		
		else if ( mState.mode == MODE_PAINT_TEXTURE ) {
			EditorCommandPaintTexture* cmd;
			if ( mTexturePaint->isApplying() ) {
				cmd = dynamic_cast<EditorCommandPaintTexture*>( mCommandQueue.getCurrentCommand() );
				if ( cmd ) {
					//console() << "Adding position to current texture paint command" << std::endl;
					cmd->addPosition( mTexturePaint->position() );
				}
			}
			else {
				//console() << "Creating new texture paint command" << std::endl;
				cmd = new EditorCommandPaintTexture( mTexturePaint, mState.texturePaint );
				cmd->addPosition( mTexturePaint->position() );
				mCommandQueue.addCommand( cmd );
			}
		}
	}
	mDragWasActive = drag.isActive;
}

void Editor::selectStraightLine( ci::Vec3f origin, ci::Vec3f target, bool constrain )
{
	//if ( !mDidPaintStraightLine ) {
		mDidPaintStraightLine = true;
		
		Vec3f direction = target - origin;
		
		if ( constrain && direction != Vec3f::zero() ) {
			mActiveSelections.clear();
			
			int length = direction.length();
			direction.normalize();
			
			if ( direction.x >= 0.5f )			direction.x = 1.0f;
			else if ( direction.x <= -0.5f )	direction.x = -1.0f;
			else direction.x = 0.0f;
			
			if ( direction.z >= 0.5f )			direction.z = 1.0f;
			else if ( direction.z <= -0.5f )	direction.z = -1.0f;
			else direction.z = 0.0f;
			
			target = origin + direction * length;
		}
		
		Ray ray( origin, target - origin );
		float maxDist = origin.distance( target );
		std::vector<EditorSelection*> selections = select( ray, mState.elevationRange, maxDist, true );
		std::vector<EditorSelection*>::iterator iter;
		for( iter = selections.begin(); iter != selections.end(); iter++) {
			(*iter)->highlight();
			mActiveSelections.push_back( *iter );
		}
	//}
}

bool Editor::setElevation( EditorSelection* selection, int elevationHeight, int terrainIndex )
{
	Vec3i tilePos = selection->getTilePosition();
	Vec3i newTilePos = Vec3i( tilePos.x, elevationHeight, tilePos.z );
	int difference = newTilePos.y - tilePos.y;
	
	if ( difference > 0 ) {
		for( int i = 0; i < difference; i++ ) {
			Block* newBlock = mGame->addBlock( tilePos + Vec3i::yAxis() * i );
			newBlock->setTerrainIndex( terrainIndex );
			newBlock->mTexturePaintMask = mTexturePaint->paintMask();
			selection->mBlock->setTerrainIndex( terrainIndex );
			selection->addBlock( newBlock );
			selection->setTilePosition( newTilePos );
			
		}
	}
	else if ( difference < 0 ) {
		/*if ( elevationHeight < 0 ) {
			selection->editingComplete();
			return false;
		}*/
		selection->setTilePosition( newTilePos );
		for( int i = 0; i < math<int>::abs( difference ); i++ ) {
			Block* oldBlock = selection->removeBlock();
			mGame->removeBlock( oldBlock );
		}
	}
	return selection->getTilePosition().y == elevationHeight;
}

std::vector<EditorSelection*> Editor::select( Ray ray, int range, float maxDistance, bool allIntersections )
{
	std::vector<EditorSelection*> output;
	std::vector<EditorSelection*>::iterator iter;
	if ( !allIntersections ) {
		//std::sort( mSelections.begin(), mSelections.end(), EditorSelection::sortCameraDistance );
	}
	for( iter = mSelections.begin(); iter != mSelections.end(); iter++) {
		bool objectHit = (*iter)->pick( ray );
		float distance = (*iter)->boundingBoxCenter().distance( ray.getOrigin() );
		if ( distance > maxDistance ) {
			objectHit = false;
		}
		if ( objectHit ) {
			EditorSelection* centerSelection = *iter;
			output.push_back( centerSelection );
			
			int n = range / 2;
			int start = -n;
			int end = range % 2 == 0 ? n-1 : n;
			for( int z = start; z <= end; z++ ) {
				for( int x = start; x <= end; x++ ) {
					if ( x == 0 && z == 0 ) continue;
					Vec3f offset = Vec3f( x, 0, z );
					
					// Find surrounding blocks
					Vec3i tilePos = centerSelection->getTilePosition() + offset;
					std::vector<EditorSelection*> allSurroundingSelections;
					for( std::vector<EditorSelection*>::iterator iter = mSelections.begin(); iter != mSelections.end(); iter++ ) {
						if ( (*iter)->getTilePosition().x == tilePos.x && (*iter)->getTilePosition().z == tilePos.z ) {
							allSurroundingSelections.push_back( *iter );
							break;
						}
					}
					
					EditorSelection* surrounding;
					if ( allSurroundingSelections.size() == 0 ) {
						continue;
					} else if ( allSurroundingSelections.size() > 1 ) {
						std::sort( allSurroundingSelections.begin(), allSurroundingSelections.end(), EditorSelection::sortHeight );
					}
					
					surrounding = *(allSurroundingSelections.end()-1);
					output.push_back( surrounding );
				}
			}
		}
		if ( objectHit && !allIntersections ) {
			return output;
		}
	}

	return output;
}

void Editor::draw()
{
	gl::color( 1, 1, 1, 1 );
	gl::setMatrices( mCamera->cinderCamera() );
	
	
	
	if ( mState.mode == MODE_PAINT_TEXTURE ) {
		mTexturePaint->draw();
	}
	else if ( mState.mode == MODE_PAINT_ELEVATION ) {
		std::vector<EditorSelection*>::iterator iter;
		for( iter = mSelections.begin(); iter != mSelections.end(); iter++) {
			EditorSelection* selection = (*iter);
			selection->draw( mState.elevationHeight, mState.showGrid );
		}
	}
	
	gl::setMatricesWindow( app::getWindowSize(), true );
	
	//mTexturePaint->debugDraw();
	 
	mPanel->draw();
}

