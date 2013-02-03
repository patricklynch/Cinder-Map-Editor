#pragma once

#include "Terrain.h"
#include "Block.h"
#include "InputButtons.h"
#include "EditorState.h"
#include "EditorMeshSelector.h"

#include "cinder/Vector.h"
#include "cinder/AxisAlignedBox.h"

#include <vector>

namespace ly {
	
class Editor;
	
class EditorSelection {
public:
	
									EditorSelection( Block* block, Editor* editor );
	
	void							setSelectionMode( SelectionMode selectionMode );
	ci::Vec3f						position;
	ci::Vec3f						tilePosition;
	
	void							update( const float deltaTime );
	void							draw( int targetElevation, bool gridLines = false );
	
	bool							pick( ci::Ray );
	void							unhighlight() { mIsHighlighted = false; }
	void							highlight() { mIsHighlighted = true; }
	bool							isHighlighted() const { return mIsHighlighted; }
	
	void							editingComplete();
	void							editingStarted();
	
	static bool						sortCameraDistance( EditorSelection* a, EditorSelection* b );
	static bool						sortHeight( EditorSelection* a, EditorSelection* b );
	float							cameraDistance();
	void							defineBoundingBox();
	
	void							resetTilePosition( ci::Vec3f iPosition );
	Block*							mBlock;
	std::vector<Block*>				mBlockStack;
	bool							mHasBeenEdited;
	BlockMeshType					mTopBlockMeshType;
	void							findSurroundingBlocks( std::vector<EditorSelection*>& selections );
	void							addBlock( Block* block );
	Block* 							removeBlock();
	bool							checkSingleBlock();
	ci::Vec3f						boundingBoxCenter() const { return mBoundingBoxCenter; }
	
private:
	ci::Vec3f						mBoundingBoxCenter;
	ci::AxisAlignedBox3f			mBoundingBox;
	EditorMeshSelector				mMeshSelector;
	Editor*							mEditor;
	int								selectionMode;
	void							updateMesh( Block* block );
	bool							mIsHighlighted;
	int								mSelectionMode;
};

}