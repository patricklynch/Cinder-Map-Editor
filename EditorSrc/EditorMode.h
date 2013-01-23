#pragma once

#include "Input.h"
#include "Tile.h"
#include "EditorCamera.h"
#include "EditorCommand.h"
#include "InputButtons.h"
#include "EditableTile.h"
#include "EditorTypes.h"
#include "EditorPanel.h"

#include "cinder/Vector.h"
#include "cinder/AxisAlignedBox.h"

#include <vector>

namespace ly {

class Game;
class Camera;
	
class EditorMode {
public:
	EditorMode( Game* game );
	virtual ~EditorMode();
	
	void draw();
	void update( const float deltaTime );
	void onButtonUp( int buttonTag );
	void onButtonDown( int buttonTag );
	
	void setMode( int index );
	void setCurrentTextureLoc( int x, int y );
	void setCurrentElevation( float value );
	void setMultiSelect( bool value ) { mMultiSelect = value; }
	
	
	EditableTile* selectedTile() const;
	EditorCamera mEditorCamera;
	
	void addCommand( EditorCommand* command );
	void redo();
	void undo();
	bool canUndo() const { return mCommandQueue.size() > 0 && currentCommand >= mCommandQueue.begin(); }
	bool canRedo() const { return mCommandQueue.size() > 0 && currentCommand < mCommandQueue.end()-1; }
	
private:
	EditorPanel mEditorPanel;
	
	ci::Vec2i mCurrentTextureLoc;
	float mCurrentElevation;
	bool mMultiSelect;
	
	std::vector<EditorCommand*> mCommandQueue;
	std::vector<EditorCommand*>::iterator currentCommand;
	
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