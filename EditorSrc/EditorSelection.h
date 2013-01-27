#pragma once

#include "Terrain.h"
#include "Block.h"
#include "InputButtons.h"
#include "EditorTypes.h"

#include "cinder/Vector.h"
#include "cinder/AxisAlignedBox.h"

#include <vector>

namespace ly {
	
struct MeshPositionData {
	bool top;
	bool bottom;
	bool horizontal;
	bool vertical;
	bool br;
	bool bc;
	bool bl;
	bool mr;
	bool ml;
	bool tr;
	bool tc;
	bool tl;
	bool rowl;
	bool rowr;
	bool rowt;
	bool rowb;
};
	
typedef enum {
	NONE = -1, BL, BC, BR, ML, MR, TL, TC, TR
} SurroundingType;

class EditorSelection {
public:
	enum							{ SELECTION_POINT, SELECTION_BLOCK };
	
									EditorSelection( Block* block );
	
	ci::Vec3f						position;
	ci::Vec3f						tilePosition;
	
	void							update( const float deltaTime );
	void							draw();
	
	bool							pick( ci::Ray );
	void							deselect() { mIsSelected = false; }
	void							select() { mIsSelected = true; }
	bool							isSelected() const { return mIsSelected; }
	void							toggleSelect() { mIsSelected = !mIsSelected; }
	void							editingComplete();
	void							editingStarted();
	
	static bool						sortCameraDistance( EditorSelection* a, EditorSelection* b );
	static bool						sortHeight( EditorSelection* a, EditorSelection* b );
	float							cameraDistance();
	void							defineBoundingBox();
	
	void							resetTilePosition( ci::Vec3f iPosition );
	Block*							mBlock;
	bool							mHasBeenEdited;
	BlockMeshType					mBlockMeshType;
	void							setBlockMeshType( BlockMeshType type );
	void							updateSurrounding( std::vector<EditorSelection*>& selections );
	void							updateMesh();
	bool							mNeedsSurroundingUpdate;
	bool							mIsPickable;
	int								mNumSurroundingUpdates;
	
private:
	std::vector<EditorSelection*>	mSurroundings;
	bool							mIsSelected;
	ci::AxisAlignedBox3f			mBoundingBox;
	int								mSelectionMode;
	MeshPositionData				mMeshPositionData;
};

}