#include "EditorMode.h"
#include "Game.h"
#include "Camera.h"
#include "Input.h"
#include "EditorConstants.h"

#include <algorithm>

#include "cinder/CinderMath.h"
#include "cinder/app/App.h"
#include "cinder/Vector.h"

using namespace ly;

using namespace ci;
using namespace ci::app;


EditorMode::EditorMode( Game* game ) : mGame( game ), mPaintMode( EditorPaintSelection ), mCurrentElevation( 0.0f ), mMultiSelect( false )
{
	mCamera = ly::Camera::get();
	mCurrentTextureLoc.set( 0, 0 );
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

void EditorMode::setCurrentTextureLoc( int x, int y )
{
	mCurrentTextureLoc = Vec2i( x, y );
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
	
	//gl::setMatricesWindow( app::getWindowSize(), true );
	
	mEditorPanel.draw();
	
}
void EditorMode::applyCurrentMode( ci::Vec2i screenPoint )
{
	pickObjectAtScreenPoint( screenPoint );
	if ( selectedTile() != NULL ) {
		switch( mPaintMode ) {
			case EditorPaintTexture:
				addCommand( new ChangeTileTextureCommand( selectedTile(), mCurrentTextureLoc ) );
				break;
			case EditorPaintElevation:
				addCommand( new ChangeTileElevationCommand( selectedTile(), mCurrentElevation ) );
				break;
			default:break;
		}
	}
}

void EditorMode::addCommand( EditorCommand* command )
{
	// If the current command is not the end, i.e. an undo has taken place, break off that future branch and delete it
	if ( mCommandQueue.size() > 0 && currentCommand < mCommandQueue.end()-1 ) {
		for( std::vector<EditorCommand*>::iterator iter = currentCommand+1; iter != mCommandQueue.end(); iter++ )
			delete *iter;
		mCommandQueue.erase( currentCommand+1, mCommandQueue.end() );
	}
	
	// If the command can and did perform (returns true), add to the queue, and set the current command to the end of the queue
	if ( command->execute() ) {
		if ( mCommandQueue.size() >= kMaxUndo ) {
			delete *mCommandQueue.begin();
			mCommandQueue.erase( mCommandQueue.begin() );
		}
		mCommandQueue.push_back( command );
		currentCommand = mCommandQueue.end()-1;
	}
	else delete command;
}

void EditorMode::undo()
{
	if ( !canUndo() ) return;
	(*currentCommand)->undo();
	currentCommand--;
}

void EditorMode::redo()
{
	if ( !canRedo() ) return;
	currentCommand++;
	(*currentCommand)->execute();
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