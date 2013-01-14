#pragma once

#include "Input.h"
#include "Tile.h"
#include "EditorCamera.h"
#include "InputButtons.h"
#include "EditableTile.h"

#include "cinder/Vector.h"
#include "cinder/AxisAlignedBox.h"

#include <vector>

namespace ly {

class Game;
class Camera;
	
/*class Drag {
public:
	Drag() : isActive( false ) {}
	bool isActive;
	int touchCount;
	ci::Vec2f current;
	ci::Vec2f start;
	ci::Vec2f amount() const { return current - start; }
};*/
	
class EditorMode {
public:
	EditorMode( Game* game );
	virtual ~EditorMode();
	
	void mouseDown( ci::app::MouseEvent event ) { Input::get()->mouseDown( event ); }
	void mouseMove( ci::app::MouseEvent event ) { Input::get()->mouseMove( event ); }
	void mouseDrag( ci::app::MouseEvent event ) { Input::get()->mouseDrag( event ); }
	void mouseUp  ( ci::app::MouseEvent event ) { Input::get()->mouseUp( event ); }
	void keyDown  ( ci::app::KeyEvent event )   { Input::get()->keyDown  ( event ); }
	
	void draw();
	void update( const float deltaTime );
	void onButtonUp( int buttonTag );
	void onButtonDown( int buttonTag );
	
	/*void touchesBegan( std::vector<Touch> touches );
	void touchesMoved( std::vector<Touch> touches );
	void touchesEnded( std::vector<Touch> touches );*/
	
	void setMode( int index );
	void setCurrentTexture( int index );
	void setCurrentElevation( float value );
	void setMultiSelect( bool value ) { mMultiSelect = value; }
	
	EditableTile* selectedTile() const;
	EditorCamera mEditorCamera;
	
private:
	int mCurrentTextureIndex;
	float mCurrentElevation;
	bool mMultiSelect;
	
	void applyCurrentMode( ci::Vec2i screenPoint );
	void pickObjectAtScreenPoint( ci::Vec2i screenPoint );
	
	EditorPaintMode mPaintMode;
	Game* mGame;
	ly::Camera* mCamera;
	std::vector<EditableTile*> mSelectedTiles;
	std::vector<EditableTile*> mToggledTiles;
	//Drag mCurrentDrag;
	float mCameraStartZoom;
	float mCameraStartAngle;
	float mCameraStartRotationY;
	ci::Vec3f mCameraStartPos;
	std::vector<EditableTile*> mTiles;
};

}