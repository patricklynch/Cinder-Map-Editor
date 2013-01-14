#pragma once

#include "Tile.h"
#include "InputButtons.h"

#include "cinder/Vector.h"
#include "cinder/AxisAlignedBox.h"

#include <vector>

namespace ly {

typedef enum {
	EditorPaintMove,
	EditorPaintSelection,
	EditorPaintTexture,
	EditorPaintElevation
} EditorPaintMode;

class EditableTile {
public:
	EditableTile( Tile* tile );
	void draw();
	void update( const float deltaTime );
	bool pick( ci::Ray );
	void deselect() { mIsSelected = false; }
	void select() { mIsSelected = true; }
	bool isSelected() const { return mIsSelected; }
	void toggleSelect() { mIsSelected = !mIsSelected; }
	float cameraDistance();
	static bool sortCameraDistance( EditableTile* a, EditableTile* b );
	void setTexture( int index );
	void setElevation( float value );
private:
	bool mIsSelected;
	Tile* mTile;
	ci::AxisAlignedBox3f mBoundingBox;
};

}