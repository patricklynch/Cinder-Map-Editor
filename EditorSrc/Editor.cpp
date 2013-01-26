#include "Editor.h"
#include "Camera.h"
#include "Input.h"
#include "Terrain.h"
#include "EditorConstants.h"

#include <algorithm>

#include "cinder/CinderMath.h"
#include "cinder/app/App.h"
#include "cinder/Vector.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

static const ci::Vec3f kPointSelectionOffset = Vec3f( 0.5f, 0.0f, 0.5f );

Editor::Editor( Game* game ) : mGame( game )
{
	mMode = ModeTerrainPaint;
	
	mCamera = ly::Camera::get();
	mBoundingCube = AssetManager::get()->getVboMesh( "models/cube.obj" );
	
	int n = kDefaultMaxVisibleTileRadius;
	
	std::vector<Block*>& blocks = mGame->blocks();
	for( int z = -n; z < n; z++ ) {
		//for( int y = -n; y < n; y++ ) {
			for( int x = -n; x < n; x++ ) {
				int y = -1;
				Vec3f tilePos = Vec3f( x, y, z );
				Block* block = findBlock( blocks, tilePos );
				{
					EditorSelection* selection = new EditorSelection( tilePos );
					mSelections.push_back( selection );
					if ( block ) selection->setBlock( block );
				}
				{
					Vec3f offsetTilePos = tilePos + kPointSelectionOffset;
					EditorSelection* selection = new EditorSelection( offsetTilePos );
					selection->isPoint = true;
					mSelections.push_back( selection );
				}
			}
		//}
	}
	
	mGame->mDelegate = this;
}

void Editor::mapCenterDidUpdate( ci::Vec3i center )
{
	for( std::vector<EditorSelection*>::iterator iter = mSelections.begin(); iter != mSelections.end(); iter++ ) {
		EditorSelection* selection = *iter;
		Vec3f resetPosition = Game::blockResetPosition( selection->tilePosition, center, kDefaultMaxVisibleTileRadius );
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
	if ( drag != NULL && !drag->isControlDown && !drag->isAltDown) {
		int range = 1;
		if ( mMode == ModeTerrainPaint ) {
			range = mEditorPanel.brushSize;
		}
		performPicking( drag->current, range );
	}
	
	for( std::vector<EditorSelection*>::iterator iter = mSelections.begin(); iter != mSelections.end(); iter++) {
		(*iter)->update( deltaTime );
	}
}

void Editor::performPicking( ci::Vec2i screenPoint, int range )
{
	Ray ray = mCamera->rayIntoScene( screenPoint );
	std::sort( mSelections.begin(), mSelections.end(), EditorSelection::sortCameraDistance );
	std::vector<EditorSelection*>::iterator iter;
	bool blockPoints = range % 2 != 0;
	for( iter= mSelections.begin(); iter != mSelections.end(); iter++) {
		(*iter)->deselect();
	}
	for( iter = mSelections.begin(); iter != mSelections.end(); iter++) {
		if ( blockPoints && (*iter)->isPoint ) continue;
		if ( !blockPoints && !(*iter)->isPoint ) continue;
		if ( (*iter)->pick( ray ) ) {
			select( *iter, range );
			return;
		}
	}
}

void Editor::select( EditorSelection* block, int range )
{
	if ( !block->isPoint ) block->select();
	int n = range / 2;
	int start = -n;
	int end = block->isPoint ? n-1 : n;
	for( int z = start; z <= end; z++ ) {
		for( int x = start; x <= end; x++ ) {
			if ( !block->isPoint && x == 0 && z == 0 ) continue;
			Vec3f offset = Vec3f( x, 0, z );
			if ( block->isPoint ) offset += kPointSelectionOffset;
			EditorSelection* surrounding = findBlock( mSelections, block->tilePosition + offset );
			if ( surrounding && !surrounding->isPoint ) {
				surrounding->select();
			}
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

