#pragma once

#include "Terrain.h"
#include "Block.h"
#include "InputButtons.h"
#include "EditorTypes.h"

#include "cinder/Vector.h"
#include "cinder/AxisAlignedBox.h"

#include <vector>

namespace ly {

class EditorSelection {
public:
	enum							{ SELECTION_POINT, SELECTION_BLOCK };
	
									EditorSelection( ci::Vec3f iPosition );
	
	ci::Vec3f						position;
	ci::Vec3f						tilePosition;
	
	void							update( const float deltaTime );
	void							draw();
	
	bool							pick( ci::Ray );
	void							deselect() { mIsSelected = false; }
	void							select() { mIsSelected = true; }
	bool							isSelected() const { return mIsSelected; }
	void							toggleSelect() { mIsSelected = !mIsSelected; }
	
	float							cameraDistance();
	static bool						sortCameraDistance( EditorSelection* a, EditorSelection* b );
	void							defineBoundingBox();
	
	void							setBlock( Block* block ) { mBlock = block; }
	
	bool							isPoint;
	void							resetTilePosition( ci::Vec3f iPosition );
	
private:
	Block*							mBlock;
	bool							mIsSelected;
	ci::AxisAlignedBox3f			mBoundingBox;
	int								mSelectionMode;
};

}