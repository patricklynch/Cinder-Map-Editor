#include "EditorMode.h"
#include "Game.h"
#include "Camera.h"
#include "Input.h"

#include <algorithm>

#include "cinder/CinderMath.h"
#include "cinder/app/App.h"
#include "cinder/Vector.h"

using namespace ly;

using namespace ci;
using namespace ci::app;


EditorMode::EditorMode( Game* game ) : mGame( game ), mPaintMode( EditorPaintMove ), mCurrentElevation( 0.0f ), mMultiSelect( false )
{
	mCamera = ly::Camera::get();
	mGame->setEditorModeActive( true );
	
	std::vector<Tile*>& gameTiles = mGame->tiles();
	for( std::vector<Tile*>::iterator iter = gameTiles.begin(); iter != gameTiles.end(); iter++) {
		mTiles.push_back( new EditableTile( *iter ) );
	}
}

EditorMode::~EditorMode() {}

void EditorMode::setMode( int index )
{
	mPaintMode = (EditorPaintMode) index;
}

void EditorMode::setCurrentTexture( int index )
{
	mCurrentTextureIndex = index;
}

void EditorMode::setCurrentElevation( float value )
{
	mCurrentElevation = value;
}

void EditorMode::update( const float deltaTime )
{
	mEditorCamera.update( deltaTime );
	
	MouseDrag* drag;
	Input* input = Input::get();
	
	drag = input->mouseDrag( Input::MOUSE_LEFT );
	if (drag != NULL && !drag->isControlDown && !drag->isAltDown) {
		applyCurrentMode( drag->current );
	}
	
	for( std::vector<EditableTile*>::iterator iter = mTiles.begin(); iter != mTiles.end(); iter++)
		(*iter)->update( deltaTime );
	
}

void EditorMode::onButtonDown( int buttonTag ){}

void EditorMode::onButtonUp( int buttonTag ) {}

void EditorMode::draw()
{
	
	gl::color( 1, 1, 1, 1 );
	for( std::vector<EditableTile*>::iterator iter = mTiles.begin(); iter != mTiles.end(); iter++)
		(*iter)->draw();
	
	gl::disableDepthRead();
	
	gl::setMatricesWindow( app::getWindowSize(), true );
	
}
void EditorMode::applyCurrentMode( ci::Vec2i screenPoint )
{
	pickObjectAtScreenPoint( screenPoint );
	if ( selectedTile() != NULL ) {
		switch( mPaintMode ) {
			case EditorPaintTexture:
				selectedTile()->setTexture( mCurrentTextureIndex );
				break;
			case EditorPaintElevation:
				selectedTile()->setElevation( mCurrentElevation );
				break;
			default:break;
		}
	}
}

EditableTile* EditorMode::selectedTile() const
{
	if ( mSelectedTiles.size() == 0 ) return NULL;
	else return mSelectedTiles[0];
}

void EditorMode::pickObjectAtScreenPoint( ci::Vec2i screenPoint )
{
	Ray ray = mCamera->rayIntoScene( screenPoint );
	bool objectPicked = false;
	std::sort( mTiles.begin(), mTiles.end(), EditableTile::sortCameraDistance );
	for( std::vector<EditableTile*>::iterator iter = mTiles.begin(); iter != mTiles.end(); iter++) {
		if ( mMultiSelect ) {
			if ( (*iter)->pick( ray ) ) {
				(*iter)->toggleSelect();
				mToggledTiles.push_back( *iter );
				if ( (*iter)->isSelected() )
					mSelectedTiles.push_back( *iter );
			}
		}
		else {
			if ( objectPicked || !(*iter)->pick( ray ) ) {
				(*iter)->deselect();
			}
			else {
				(*iter)->select();
				objectPicked = true;
				mSelectedTiles.clear();
				mSelectedTiles.push_back( *iter );
			}
		}
	}
}