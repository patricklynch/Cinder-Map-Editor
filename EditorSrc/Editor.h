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
	
	void									removeSelectionForBlock( Block* block );
	
private:
	EditorCamera							mEditorCamera;
	EditorPanel								mEditorPanel;
	EditorCommandQueue						mCommandQueue;
	
	MouseDrag*								mLastDrag;
	
	ci::gl::VboMesh*						mBoundingCube;
	std::vector<EditorSelection*>			select( ci::Vec2i screenPoint, int range, bool showHighlight = true );
	void									updateMeshes( std::vector<EditorSelection*>& selections );
	
	EditorMode								mMode;
	Game*									mGame;
	ly::Camera*								mCamera;
	std::vector<EditorSelection*>			mSelections;
	std::vector<EditorSelection*>			mSelectionsToUpdate;
	
	/** This property keeps the elevation (tilePosition.y) applied to each new block the same as the
		block first selected when editing beings.  This gives more control to users by preventing
		unwanted pileup of blocks when overlapping elevated area */
	int										mBlockTargetElevation;
	
	// Dragging properties
	float									mCameraStartZoom;
	float									mCameraStartAngle;
	float									mCameraStartRotationY;
	ci::Vec3f								mCameraStartPos;
};

}