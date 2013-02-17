#include "EditorMeshSelector.h"
#include "EditorSelection.h"
#include "GameConstants.h"

#include "cinder/app/App.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

EditorMeshSelector::EditorMeshSelector() {}

EditorMeshSelector::~EditorMeshSelector() {}

void EditorMeshSelector::setSurrounding( std::vector<EditorSelection*>& selections, ci::Vec3i centerTilePosition )
{
	mSurroundings.clear();
	for( int i = 0; i < 8; i++ ) {
		mSurroundings.push_back( NULL );
	}
	
	int  n = kDefaultMaxVisibleTileRadius * 2;
	
	for( std::vector<EditorSelection*>::iterator iter = selections.begin(); iter != selections.end(); iter++) {
		EditorSelection* edge = *iter;
		Vec2i relPos = edge->getTilePosition().xz() - centerTilePosition.xz();
		SurroundingType index = NONE;
		
		if (relPos == Vec2i( -1, -1 ) || relPos == Vec2i( n,  n ) ||
			relPos == Vec2i( -1,  n ) || relPos == Vec2i( n, -1 ) ) index = BL;
		
		if (relPos == Vec2i(  0, -1 ) || relPos == Vec2i(  0,  n ) ) index = BC;
		
		if (relPos == Vec2i(  1, -1 ) || relPos == Vec2i( -n,  n ) ||
			relPos == Vec2i( -n, -1 ) || relPos == Vec2i(  1,  n ) ) index = BR;
		
		if (relPos == Vec2i( -1,  0 ) || relPos == Vec2i(  n,  0 ) ) index = ML;
		
		if (relPos == Vec2i(  1,  0 ) || relPos == Vec2i( -n,  0 ) ) index = MR;
		
		if (relPos == Vec2i( -1,  1 ) || relPos == Vec2i(  n, -n ) ||
			relPos == Vec2i(  n,  1 ) || relPos == Vec2i( -1, -n ) ) index = TL;
		
		if (relPos == Vec2i(  0,  1 ) || relPos == Vec2i(  0, -n ) ) index = TC;
		
		if (relPos == Vec2i(  1,  1 ) || relPos == Vec2i( -n, -n ) ||
			relPos == Vec2i(  1, -n ) || relPos == Vec2i( -n,  1 ) ) index = TR;
		
		if ( index != NONE ) {
			mSurroundings[ (int) index ] = *iter;
		}
	}
}

bool EditorMeshSelector::check( EditorSelection* surrounding, Block* block )
{
	Block* surroundingBlock = surrounding->getBlockAtElevation( block->tilePosition.y );
	return  //surrounding->mBlock->tilePosition.y >= block->tilePosition.y &&
			surroundingBlock != NULL; // &&
			//surroundingBlock->meshType() != BlockMeshNone;
}

EditorMeshSelectorResult EditorMeshSelector::getMeshSelection( Block* block )
{
	EditorMeshSelectorResult result;

	// Collect data in a nice short hand form for easier syntax when we analyze it
	// A true value for any property indicates that the space or spaces are filled
	// 'tr' means top right is filled, 'rowt' means to the whole top row is filled, 'horizontal' means the middle row is filled
	MeshPositionData& md	=	mMeshPositionData;
	md.top					=	check( mSurroundings[TL], block ) &&
								check( mSurroundings[TC], block ) &&
								check( mSurroundings[TR], block );
	md.bottom				=	check( mSurroundings[BL], block ) &&
								check( mSurroundings[BC], block ) &&
								check( mSurroundings[BR], block );
	md.horizontal			=	check( mSurroundings[MR], block ) &&
								check( mSurroundings[ML], block );
	md.vertical				=	check( mSurroundings[TC], block ) &&
								check( mSurroundings[BC], block );
	md.br					=	check( mSurroundings[BR], block );
	md.bc					=	check( mSurroundings[BC], block );
	md.bl					=	check( mSurroundings[BL], block );
	md.mr					=	check( mSurroundings[MR], block );
	md.ml					=	check( mSurroundings[ML], block );
	md.tr					=	check( mSurroundings[TR], block );
	md.tc					=	check( mSurroundings[TC], block );
	md.tl					=	check( mSurroundings[TL], block );
	md.rowl					=	check( mSurroundings[BL], block ) &&
								check( mSurroundings[ML], block ) &&
								check( mSurroundings[TL], block );
	md.rowr					=	check( mSurroundings[BR], block ) &&
								check( mSurroundings[MR], block ) &&
								check( mSurroundings[TR], block );
	md.rowt					=	check( mSurroundings[TL], block ) &&
								check( mSurroundings[TC], block ) &&
								check( mSurroundings[TR], block );
	md.rowb					=	check( mSurroundings[BL], block ) &&
								check( mSurroundings[BC], block ) &&
								check( mSurroundings[BR], block );
	
	BlockMeshType newMeshType = BlockMeshNone;
	float rotationY = 0.0f;
	
	// Solid cube when compeltely surrounded
	
	if ( md.rowt && md.horizontal && md.rowb && md.rowr && md.rowl ) {
		newMeshType = BlockMeshCenter;
		rotationY = 0.0f;
	}
	
	else if ( md.horizontal && md.vertical ) {
		
		// Inner corner mesh and rotation
		if ( md.rowb && !md.tr && md.tl && md.br && md.bl ) {
			newMeshType = BlockMeshInnerCorner;
			rotationY = 180.0f;
		}
		else if ( md.rowb && !md.tl && md.tr && md.br && md.bl ) {
			newMeshType = BlockMeshInnerCorner;
			rotationY = 90.0f;
		}
		else if ( md.rowt && !md.br && md.tl && md.tr && md.bl ) {
			newMeshType = BlockMeshInnerCorner;
			rotationY = -90.0f;
		}
		else if ( md.rowt && !md.bl && md.tl && md.br && md.tr ) {
			newMeshType = BlockMeshInnerCorner;
			rotationY = 0.0f;
		}
		
		// Bridge mesh and rotation
		else if ( md.tr && md.bl ) {
			newMeshType = BlockMeshBridge;
			rotationY = 0.0f;
		}
		else if ( md.br && md.tl ) {
			newMeshType = BlockMeshBridge;
			rotationY = 90.0f;
		}
	}
	
	// Wall mesh and rotation
	else if ( md.horizontal && md.rowt && !md.bc ) {
		newMeshType = BlockMeshWall;
		rotationY = -90.0f;
	}
	else if ( md.horizontal && md.rowb && !md.tc ) {
		newMeshType = BlockMeshWall;
		rotationY = 90.0f;
	}
	else if ( md.vertical && md.rowr && !md.ml ) {
		newMeshType = BlockMeshWall;
		rotationY = 0.0f;
	}
	else if ( md.vertical && md.rowl && !md.mr ) {
		newMeshType = BlockMeshWall;
		rotationY = 180.0f;
	}
	
	// Corner mesh and rotation
	else if ( md.mr && md.tc && md.tr && !md.ml && !md.bc ) {
		newMeshType = BlockMeshCorner;
		rotationY = 0.0f;
	}
	else if ( md.ml && md.tc && md.tl && !md.mr && !md.bc ) {
		newMeshType = BlockMeshCorner;
		rotationY = -90.0f;
	}
	else if ( md.mr && md.bc && md.br && !md.ml && !md.tc ) {
		newMeshType = BlockMeshCorner;
		rotationY = 90.0f;
	}
	else if ( md.ml && md.bc && md.bl && !md.mr && !md.tc ) {
		newMeshType = BlockMeshCorner;
		rotationY = 180.0f;
	}
	
	else if ( md.horizontal && md.tc && md.tr ) {
		newMeshType = BlockMeshCorner;
		rotationY = 0.0f;
	}
	else if ( md.horizontal && md.bc && md.br ) {
		newMeshType = BlockMeshCorner;
		rotationY = 90.0f;
	}
	else if ( md.horizontal && md.tc && md.tl ) {
		newMeshType = BlockMeshCorner;
		rotationY = -90.0f;
	}
	else if ( md.horizontal && md.bc && md.bl ) {
		newMeshType = BlockMeshCorner;
		rotationY = 180.0f;
	}
	
	else if ( md.vertical&& md.br && md.mr ) {
		newMeshType = BlockMeshCorner;
		rotationY = 90.0f;
	}
	else if ( md.vertical && md.bl && md.ml ) {
		newMeshType = BlockMeshCorner;
		rotationY = 180.0f;
	}
	else if ( md.vertical && md.tr && md.mr ) {
		newMeshType = BlockMeshCorner;
		rotationY = 0.0f;
	}
	else if ( md.vertical && md.tl && md.ml ) {
		newMeshType = BlockMeshCorner;
		rotationY = -90.0f;
	}
	
	// Remove any single straggler blocks that won't connect properly
	// Setting to BlockMeshNone will flag it for removal
	else if ( !md.rowt && !md.rowb && !md.rowl && !md.rowr ) {
		newMeshType = BlockMeshNone;
	}
	else if ( !md.rowt && !md.horizontal ) {
		newMeshType = BlockMeshNone;
	}
	else if ( !md.rowb && !md.horizontal ) {
		newMeshType = BlockMeshNone;
	}
	else if ( !md.rowl && !md.vertical ) {
		newMeshType = BlockMeshNone;
	}
	else if ( !md.rowr && !md.vertical ) {
		newMeshType = BlockMeshNone;
	}
	
	// If no ohter mesh count be found, remove it
	else {
		newMeshType = BlockMeshNone;
	}
	
	result.type = newMeshType;
	result.rotation = rotationY;
	
	return result;
}