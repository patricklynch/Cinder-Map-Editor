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

Editor::Editor( Game* game ) : mGame( game )
{
	mMode = MODE_PAINT_ELEVATION;
	
	mCamera = ly::Camera::get();
	mCamera->setFov( 50 );
	mCamera->setZoom( 20 );
	//mCamera->rotation.y = 0.0f;
	mCamera->setAngle( -80.0f);
	mCamera->rotation.y = 45.0f;
	mCamera->setAngle( -45.0f);
	
	// Create EditorSeletion as a wrapper for each block in the game
	std::vector<Block*>& blocks = mGame->blocks();
	for( std::vector<Block*>::iterator iter = blocks.begin(); iter != blocks.end(); iter++ ) {
		mSelections.push_back( new EditorSelection( *iter, this ) );
	}
	// Set references to each block's surrounding blocks
	for( std::vector<EditorSelection*>::iterator iter = mSelections.begin(); iter != mSelections.end(); iter++) {
		(*iter)->findSurroundingBlocks( mSelections );
	}
	
	mPanel = new EditorPanel( this );
	currentElevationTarget = kUndefinedElevation;
	mGame->mDelegate = this;
	mKeyCommands = new EditorKeyCommands( this );
	
	mDidPaintStraightLine = false;
}

Editor::~Editor()
{
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
		Vec3f resetPosition = Game::blockResetPosition( selection->tilePosition, center, kDefaultMaxVisibleTileRadius );
		selection->resetTilePosition( resetPosition );
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
		if ( (*iter)->tilePosition == tilePos ) {
			return *iter;
		}
	}
	return NULL;
}

void Editor::update( const float deltaTime )
{
	mEditorCamera.update( deltaTime );
	
	applyUserEdits();
	
	SelectionMode selectionMode = (mState.brushSize % 2 == 0) ? SELECTION_POINT : SELECTION_FACE;
	
	// Update the selection objects
	std::vector<EditorSelection*>::iterator iter;
	for( iter = mSelections.begin(); iter != mSelections.end(); iter++) {
		(*iter)->setSelectionMode( selectionMode );
		(*iter)->update( deltaTime );
	}
	
	// Purge and blocks with BlockMeshNone
	for( iter = mSelections.begin(); iter != mSelections.end(); iter++) {
		if ( (*iter)->mTopBlockMeshType == BlockMeshNone ) {
			setElevation( *iter, (*iter)->tilePosition.y-1 );
			(*iter)->update( deltaTime );
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
	if ( mMode == MODE_PAINT_ELEVATION ) {
		clearActiveSelections();
		Ray ray = mCamera->rayIntoScene( Input::get()->mousePosition() );
		mActiveSelections = select( ray, mState.brushSize );
		std::vector<EditorSelection*>::iterator iter;
		for( iter = mActiveSelections.begin(); iter != mActiveSelections.end(); iter++) {
			(*iter)->highlight();
		}
	}
	
	Input* input = Input::get();
	MouseDrag& drag = input->getMouseDrag();
	if ( !drag.isActive && drag.mouseButton == MOUSE_LEFT ) {
		if ( mMode == MODE_PAINT_ELEVATION ) {
			// If the editing just stopped
			if ( mDragWasActive ) {
				for( std::vector<EditorSelection*>::iterator iter = mSelections.begin(); iter != mSelections.end(); iter++ ) {
					(*iter)->editingComplete();
				}
				currentElevationTarget = kUndefinedElevation;
				mDidPaintStraightLine = false;
			}
		}
	}
	else if ( drag.isActive && drag.mouseButton == MOUSE_LEFT ) {
		if ( mMode == MODE_PAINT_ELEVATION ) {
			
			// Find all selections between here and the current selection point and add to mActiveSelections
			if ( drag.isShiftDown && mLastSelectionTarget != kTargetUndefined) {
				selectStraightLine( mLastSelectionTarget, mActiveSelections[0]->boundingBoxCenter() );
			}
			
			if ( mActiveSelections.size() > 0 ) {
				if ( currentElevationTarget == kUndefinedElevation ) {
					currentElevationTarget = mState.targetElevation;
				}
				
				EditorCommandModifyElevation* cmd = new EditorCommandModifyElevation();
				cmd->activeSelections = mActiveSelections;
				cmd->editor = this;
				cmd->amount = currentElevationTarget;
				mCommandQueue.addCommand( cmd );
			}
		}
		if (mActiveSelections.size() > 0 ) {
			mLastSelectionTarget = mActiveSelections[0]->boundingBoxCenter();
		} else {
			mLastSelectionTarget = kTargetUndefined;
		}
	}
	mDragWasActive = drag.isActive;
}

void Editor::selectStraightLine( ci::Vec3f origin, ci::Vec3f target )
{
	if ( !mDidPaintStraightLine ) {
		mDidPaintStraightLine = true;
		Ray ray( origin, target - origin );
		float maxDist = origin.distance( target );
		std::vector<EditorSelection*> selections = select( ray, mState.brushSize, maxDist, true );
		std::vector<EditorSelection*>::iterator iter;
		for( iter = selections.begin(); iter != selections.end(); iter++) {
			(*iter)->highlight();
			mActiveSelections.push_back( *iter );
		}
	}
}

bool Editor::setElevation( EditorSelection* selection, int targetElevation )
{
	Vec3i tilePos = selection->tilePosition;
	Vec3i newTilePos = Vec3i( tilePos.x, targetElevation, tilePos.z );
	int difference = newTilePos.y - tilePos.y;
	
	if ( difference > 0 ) {
		for( int i = 0; i < difference; i++ ) {
			Block* newBlock = mGame->addBlock( tilePos + Vec3i::yAxis() * i );
			selection->addBlock( newBlock );
			selection->resetTilePosition( newTilePos );
			
		}
	}
	else if ( difference < 0 ) {
		/*if ( tilePos.y < 0 ) {
			selection->editingComplete();
			return false;
		}*/
		selection->resetTilePosition( newTilePos );
		for( int i = 0; i < selection->mBlockStack.size(); i++ ) {
			Block* oldBlock = selection->removeBlock();
			mGame->removeBlock( oldBlock );
		}
	}
	return selection->tilePosition.y == targetElevation;
}

std::vector<EditorSelection*> Editor::select( Ray ray, int range, float maxDistance, bool allIntersections )
{
	std::vector<EditorSelection*> output;
	std::vector<EditorSelection*>::iterator iter;
	for( iter = mSelections.begin(); iter != mSelections.end(); iter++) {
		bool objectHit = (*iter)->pick( ray );
		float distance = (*iter)->boundingBoxCenter().distance( ray.getOrigin() );
		if ( allIntersections ) console() << "Distance = " << distance << ", max = " << maxDistance << std::endl;
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
					Vec3i tilePos = centerSelection->tilePosition + offset;
					std::vector<EditorSelection*> allSurroundingSelections;
					for( std::vector<EditorSelection*>::iterator iter = mSelections.begin(); iter != mSelections.end(); iter++ ) {
						if ( (*iter)->tilePosition.x == tilePos.x && (*iter)->tilePosition.z == tilePos.z ) {
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
			if ( objectHit && !allIntersections ) {
				return output;
			}
		}
	}
	
	return output;
}

void Editor::draw()
{
	gl::color( 1, 1, 1, 1 );
	gl::setMatrices( mCamera->cinderCamera() );
	
	gl::enableWireframe();
	
	std::vector<EditorSelection*>::iterator iter;
	for( iter = mSelections.begin(); iter != mSelections.end(); iter++) {
		EditorSelection* selection = (*iter);
		selection->draw( mState.targetElevation, mState.showGrid );
	}
	
	gl::disableWireframe();
	
	gl::setMatricesWindow( app::getWindowSize(), true );
	
	mPanel->draw();
}

