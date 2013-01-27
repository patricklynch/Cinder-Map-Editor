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

static const ci::Vec3f kPointSelectionOffset = Vec3f( 0.5f, 0.0f, 0.5f );

Editor::Editor( Game* game ) : mGame( game )
{
	mMode = ModeTerrainPaint;
	
	mCamera = ly::Camera::get();
	mCamera->setFov( 45 );
	mCamera->setZoom( 20 );
	mCamera->rotation.y = 0.0f;
	mCamera->setAngle( -90.0f);
	
	mBoundingCube = AssetManager::get()->getVboMesh( "models/cube.obj" );
	
	std::vector<Block*>& blocks = mGame->blocks();
	for( std::vector<Block*>::iterator iter = blocks.begin(); iter != blocks.end(); iter++ ) {
		createSelectionFromBlock( *iter );
	}
	
	mGame->mDelegate = this;
}

void Editor::createSelectionFromBlock( Block* block )
{
	EditorSelection* selection = findBlock( mSelections, block->tilePosition );
	if ( selection == NULL ) {
		mSelections.push_back( new EditorSelection( block->tilePosition, block) );
	}
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
		selection->mNeedsSurroundingUpdate = (int) selection->tilePosition.distance( center ) > kDefaultMaxVisibleTileRadius - 2;
		selection->resetTilePosition( resetPosition );
	}
}

Editor::~Editor() {}

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
	
	Input* input = Input::get();
	MouseDrag* drag = input->mouseDrag( Input::MOUSE_LEFT );
	
	if ( drag == NULL ) {
		for( std::vector<EditorSelection*>::iterator iter = mSelections.begin(); iter != mSelections.end(); iter++ ) {
			EditorSelection* activeSelection = *iter;
			activeSelection->mHasBeenEdited = false;
		}
		if ( mMode == ModeTerrainPaint ) {
			// If the painting just stopped
			if ( mLastDrag != NULL ) {
				updateMeshes( mSelections );
			}
		}
	}
	else if ( drag != NULL && !drag->isControlDown && !drag->isAltDown) {
		int range = 1;
		if ( mMode == ModeTerrainPaint ) {
			range = mEditorPanel.brushSize;
			performPicking( drag->current, range );
			
			// Apply to selected items
			// TODO: Make a command and use queue
			for( std::vector<EditorSelection*>::iterator iter = mActiveSelections.begin(); iter != mActiveSelections.end(); iter++ ) {
				EditorSelection* activeSelection = *iter;
				if ( activeSelection->mHasBeenEdited ) continue;
				Vec3i tilePos = activeSelection->tilePosition;
				//tilePos.y += mEditorPanel.brushErase ? -1 : 1;
				tilePos.y = 1;
				activeSelection->resetTilePosition( tilePos );
				activeSelection->mHasBeenEdited = true;
				activeSelection->mNeedsSurroundingUpdate = true;
			}
		}
	}
	mLastDrag = drag;
	
	updateMeshes( mActiveSelections );
	
	// Update the selection objects
	for( std::vector<EditorSelection*>::iterator iter = mSelections.begin(); iter != mSelections.end(); iter++) {
		(*iter)->update( deltaTime );
	}
}

void Editor::updateMeshes( std::vector<EditorSelection*>& selections )
{
	// Update each block's references to its surrounding blocks
	for( std::vector<EditorSelection*>::iterator iter = selections.begin(); iter != selections.end(); iter++) {
		(*iter)->updateSurrounding( mSelections );
	}
	// Update the mesh selection based on surrounding blocks
	for( std::vector<EditorSelection*>::iterator iter = mSelections.begin(); iter != mSelections.end(); iter++) {
		(*iter)->updateMesh();
	}
}

void Editor::performPicking( ci::Vec2i screenPoint, int range )
{
	Ray ray = mCamera->rayIntoScene( screenPoint );
	std::sort( mSelections.begin(), mSelections.end(), EditorSelection::sortCameraDistance );
	std::vector<EditorSelection*>::iterator iter;
	for( iter= mSelections.begin(); iter != mSelections.end(); iter++) {
		(*iter)->deselect();
	}
	for( iter = mSelections.begin(); iter != mSelections.end(); iter++) {
		if ( (*iter)->pick( ray ) ) {
			select( *iter, range );
			return;
		}
	}
}

void Editor::select( EditorSelection* centerSelection, int range )
{
	mActiveSelections.clear();
	centerSelection->select();
	mActiveSelections.push_back( centerSelection );
	
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
			surrounding->select();
			mActiveSelections.push_back( surrounding );
		}
	}
}

void Editor::draw()
{
	gl::color( 1, 1, 1, 1 );
	gl::setMatrices( mCamera->cinderCamera() );
	
	gl::enableWireframe();
	
	std::vector<EditorSelection*>::iterator iter;
	for( iter = mSelections.begin(); iter != mSelections.end(); iter++) {
		EditorSelection* selection = (*iter);
		selection->draw();
	}
	gl::disableWireframe();
	
	gl::setMatricesWindow( app::getWindowSize(), true );
	
	mEditorPanel.draw();
}

