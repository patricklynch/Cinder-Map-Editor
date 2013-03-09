#pragma once

#include "Terrain.h"
#include "Tile.h"
#include "InputButtons.h"
#include "EditorState.h"

#include "cinder/Vector.h"
#include "cinder/AxisAlignedBox.h"

#include <vector>

namespace ly {
	
class Editor;

const static int kEditableTileAllElevations = -999;
	
class EditableTile {
public:
	
									EditableTile( Tile* mTile, Editor* editor );
	
	void							setSelectionMode( SelectionMode_t selectionMode );
	
	void							update( const float deltaTime = 0.0f );
	void							draw( int elevationHeight, bool gridLines = false );
	
	bool							pick( ci::Ray );
	void							unhighlight() { mIsHighlighted = false; }
	void							highlight() { mIsHighlighted = true; }
	bool							isHighlighted() const { return mIsHighlighted; }
	
	void							editingComplete();
	void							editingStarted();
	
	static bool						sortCameraDistance( EditableTile* a, EditableTile* b );
	static bool						sortHeight( EditableTile* a, EditableTile* b );
	float							cameraDistance();
	void							defineBoundingBox();
	
	bool							mHasBeenEdited;
	ci::Vec3f						boundingBoxCenter() const { return mBoundingBoxCenter; }
	
	bool							elevationDidUpdate();
	void							clearElevationUpdateFlag() { mElevationDidUpdate = false; }
	void							forceElevationUpdateFlag() { mElevationDidUpdate = true; }
	void							setTerrainIndex( int index );
	
	Tile*							getTile() const { return mTile; }
	
private:
	Tile*							mTile;
	bool							mElevationDidUpdate;
	ci::Vec3f						mBoundingBoxCenter;
	ci::AxisAlignedBox3f			mBoundingBox;
	Editor*							mEditor;
	int								selectionMode;
	void							updateMesh( Block* block );
	bool							mIsHighlighted;
	int								mSelectionMode;
};

}