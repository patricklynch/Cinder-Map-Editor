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
		Vec2i relPos = edge->tilePosition.xz() - centerTilePosition.xz();
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

EditorMeshSelectorResult EditorMeshSelector::getMeshSelection( Block* block )
{
	EditorMeshSelectorResult result;
	
	int elevation = block->tilePosition.y;
	int terrain = block->terrainIndex();

	// Collect data in a nice short hand form for easier syntax when we analyze it
	// A true value for any property indicates that the space or spaces are filled
	// 'tr' means top right is filled, 'rowt' means to the whole top row is filled, 'horizontal' means the middle row is filled
	MeshPositionData& md	=	mMeshPositionData;
	md.top					=	mSurroundings[TL]->tilePosition.y >= elevation &&
								mSurroundings[TC]->tilePosition.y >= elevation &&
								mSurroundings[TR]->tilePosition.y >= elevation;
	md.bottom				=	mSurroundings[BL]->tilePosition.y >= elevation &&
								mSurroundings[BC]->tilePosition.y >= elevation &&
								mSurroundings[BR]->tilePosition.y >= elevation;
	md.horizontal			=	mSurroundings[MR]->tilePosition.y >= elevation &&
								mSurroundings[ML]->tilePosition.y >= elevation;
	md.vertical				=	mSurroundings[TC]->tilePosition.y >= elevation &&
								mSurroundings[BC]->tilePosition.y >= elevation;
	md.br					=	mSurroundings[BR]->tilePosition.y >= elevation;
	md.bc					=	mSurroundings[BC]->tilePosition.y >= elevation;
	md.bl					=	mSurroundings[BL]->tilePosition.y >= elevation;
	md.mr					=	mSurroundings[MR]->tilePosition.y >= elevation;
	md.ml					=	mSurroundings[ML]->tilePosition.y >= elevation;
	md.tr					=	mSurroundings[TR]->tilePosition.y >= elevation;
	md.tc					=	mSurroundings[TC]->tilePosition.y >= elevation;
	md.tl					=	mSurroundings[TL]->tilePosition.y >= elevation;
	md.rowl					=	mSurroundings[BL]->tilePosition.y >= elevation &&
								mSurroundings[ML]->tilePosition.y >= elevation &&
								mSurroundings[TL]->tilePosition.y >= elevation;
	md.rowr					=	mSurroundings[BR]->tilePosition.y >= elevation &&
								mSurroundings[MR]->tilePosition.y >= elevation &&
								mSurroundings[TR]->tilePosition.y >= elevation;
	md.rowt					=	mSurroundings[TL]->tilePosition.y >= elevation &&
								mSurroundings[TC]->tilePosition.y >= elevation &&
								mSurroundings[TR]->tilePosition.y >= elevation;
	md.rowb					=	mSurroundings[BL]->tilePosition.y >= elevation &&
								mSurroundings[BC]->tilePosition.y >= elevation &&
								mSurroundings[BR]->tilePosition.y >= elevation;
	
	BlockMeshType newMeshType = BlockMeshNone;
	float rotationY = 0.0f;

	/*for( std::vector<EditorSelection*>::const_iterator iter = mSurroundings.begin(); iter != mSurroundings.end(); iter++ ) {
		EditorSelection* selection = *iter;
		if ( selection->tilePosition.y == elevation && selection->mBlock->terrainIndex() != terrain ) {
			result.type = newMeshType;
			result.rotation = rotationY;
			return result;
		}
	}*/

	if ( mSurroundings.size() <= 3 ) {
		result.type = newMeshType;
		result.rotation = rotationY;
		return result;
	}
	
	// Solid cube when compeltely surrounded
	if ( md.rowt && md.horizontal && md.rowb ) {
		newMeshType = BlockMeshCenter;
		rotationY = 0.0f;
	}
	
	else if ( md.horizontal && md.vertical ) {
		
		// Inner corner mesh and rotation
		if ( md.rowb && !md.tr ) {
			newMeshType = BlockMeshInnerCorner;
			rotationY = 180.0f;
		}
		else if ( md.rowb && !md.tl ) {
			newMeshType = BlockMeshInnerCorner;
			rotationY = 90.0f;
		}
		else if ( md.rowt && !md.br ) {
			newMeshType = BlockMeshInnerCorner;
			rotationY = -90.0f;
		}
		else if ( md.rowt && !md.bl ) {
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
	else if ( md.horizontal && md.rowt ) {
		newMeshType = BlockMeshWall;
		rotationY = -90.0f;
	}
	else if ( md.horizontal && md.rowb ) {
		newMeshType = BlockMeshWall;
		rotationY = 90.0f;
	}
	else if ( md.vertical && md.rowr) {
		newMeshType = BlockMeshWall;
		rotationY = 0.0f;
	}
	else if ( md.vertical && md.rowl ) {
		newMeshType = BlockMeshWall;
		rotationY = 180.0f;
	}
	
	// Corner mesh and rotation
	else if ( md.mr && md.tc && !md.ml && !md.bc ) {
		newMeshType = BlockMeshCorner;
		rotationY = 0.0f;
	}
	else if ( md.ml && md.tc && !md.mr && !md.bc ) {
		newMeshType = BlockMeshCorner;
		rotationY = -90.0f;
	}
	else if ( md.mr && md.bc && !md.ml && !md.tc ) {
		newMeshType = BlockMeshCorner;
		rotationY = 90.0f;
	}
	else if ( md.ml && md.bc && !md.mr && !md.tc ) {
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