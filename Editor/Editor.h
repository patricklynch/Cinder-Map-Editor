#pragma once

#include "Input.h"
#include "InputButtons.h"
#include "AssetManager.h"
#include "Game.h"

#include "EditorCamera.h"
#include "EditorCommand.h"
#include "EditorSelection.h"
#include "EditorState.h"
#include "EditorPanel.h"
#include "EditorCommandQueue.h"
#include "EditorKeyCommands.h"
#include "EditorTexturePaint.h"

#include "cinder/gl/Vbo.h"
#include "cinder/Vector.h"
#include "cinder/AxisAlignedBox.h"

#include <vector>

namespace ly {

#if _WINDOWS
#define MAXFLOAT FLT_MAX
#endif
	
class Camera;
	
static const ci::Vec3f	kPointSelectionOffset		= ci::Vec3f( 0.5f, 0.0f, 0.5f );
static const ci::Vec3f	kTargetUndefined			= ci::Vec3f( -9999, -9999, -9999 );
static const int		kUndefinedElevation			= -99999;
	
class Editor : public IGameEventDelegate {
public:
											Editor( Game* game );
	virtual									~Editor();
	
	void									draw();
	void									update( const float deltaTime );
	
	static Block*							findBlock( std::vector<Block*>& blocks, ci::Vec3i tilePos );
	static EditorSelection*					findBlock( std::vector<EditorSelection*>& blocks, ci::Vec3i tilePos );
	void									resetElevation();
		
	/** IGameEventDelegate methods */
	virtual void							mapCenterDidUpdate( ci::Vec3i center );
	
	void									removeSelectionForBlock( Block* block );
	
	EditorCamera							mEditorCamera;
	EditorPanel*							mPanel;
	EditorCommandQueue						mCommandQueue;
	EditorKeyCommands*						mKeyCommands;
	EditorState								mState;
	
	bool									setElevation( EditorSelection* selection, int elevationHeight );
	int										currentElevationTarget;
	ly::Camera*								mCamera;
	Game*									mGame;
	
private:
	ci::Vec3f								mLastSelectionTarget;
	void									applyUserEdits();
	bool									mDragWasActive;
	
	std::vector<EditorSelection*>			select( ci::Ray ray, int range, float maxDistance = MAXFLOAT, bool allIntersections = false );
	void									selectStraightLine( ci::Vec3f origin, ci::Vec3f target, bool constrain = false );
	
	std::vector<EditorSelection*>			mSelections;
	std::vector<EditorSelection*>			mActiveSelections;
	void									clearActiveSelections();
	bool									mDidPaintStraightLine;
	
	EditorTexturePaint*						mTexturePaint;
	
	// Dragging properties
	float									mCameraStartZoom;
	float									mCameraStartAngle;
	float									mCameraStartRotationY;
	ci::Vec3f								mCameraStartPos;
};

}