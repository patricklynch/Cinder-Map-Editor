#pragma once

#include "Input.h"
#include "InputButtons.h"
#include "AssetManager.h"
#include "Game.h"

#include "EditorCamera.h"
#include "EditorCommand.h"
#include "EditorSelection.h"
#include "EditorTypes.h"
#include "EditorPanel.h"
#include "EditorCommandQueue.h"

#include "cinder/gl/Vbo.h"
#include "cinder/Vector.h"
#include "cinder/AxisAlignedBox.h"

#include <vector>

namespace ly {
	
class Camera;
	
class Editor : public IGameEventDelegate {
public:
											Editor( Game* game );
	virtual									~Editor();
	
	void									draw();
	void									update( const float deltaTime );
	
	static Block*							findBlock( std::vector<Block*>& blocks, ci::Vec3i tilePos );
	static EditorSelection*					findBlock( std::vector<EditorSelection*>& blocks, ci::Vec3i tilePos );
		
	/** IGameEventDelegate methods */
	virtual void							mapCenterDidUpdate( ci::Vec3i center );
	
private:
	EditorCamera							mEditorCamera;
	EditorPanel								mEditorPanel;
	EditorCommandQueue						mCommandQueue;
	
	ci::gl::VboMesh*						mBoundingCube;
	void									performPicking( ci::Vec2i screenPoint, int range );
	void									select( EditorSelection* block, int range );
	
	EditorMode								mMode;
	Game*									mGame;
	ly::Camera*								mCamera;
	std::vector<EditorSelection*>			mSelections;
	
	// Dragging properties
	float									mCameraStartZoom;
	float									mCameraStartAngle;
	float									mCameraStartRotationY;
	ci::Vec3f								mCameraStartPos;
};

}