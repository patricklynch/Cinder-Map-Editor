#include "BlockProfileSelector.h"
#include "GameConstants.h"
#include "Tile.h"

#include "cinder/app/App.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

BlockProfileSelector::BlockProfileSelector() {}

BlockProfileSelector::~BlockProfileSelector() {}

void BlockProfileSelector::setSurrounding( std::vector<Tile*>& tiles, Tile* centerTile )
{
	mCenter = centerTile;
	mSurroundings.clear();
	for( int i = 0; i < 8; i++ ) {
		mSurroundings.push_back( NULL );
	}
	
	int  n = kDefaultMaxVisibleTileRadius * 2;
	
	for( std::vector<Tile*>::iterator iter = tiles.begin(); iter != tiles.end(); iter++) {
		Tile* edge = *iter;
		Vec2i relPos = edge->getTilePosition() - mCenter->getTilePosition();
		SurroundingBlockType index = NONE;
		
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

bool BlockProfileSelector::check( Tile* surrounding, int elevation, int terrainIndex )
{
	Block* surroundingBlock = surrounding->getBlockAtElevation( elevation );
	if ( !surroundingBlock ) {
		return false;
	}	
	else return true; //surroundingBlock->terrainIndex() == terrainIndex;
}

BlockProfile BlockProfileSelector::getProfile( Block* block )
{
	BlockProfile profile;
	
	SurroundingBlockData& md=	mSurroundingBlockData;
	md.horizontal			=	check( mSurroundings[MR], block->tilePosition.y, block->terrainIndex() ) &&
								check( mSurroundings[ML], block->tilePosition.y, block->terrainIndex() );
	md.vertical				=	check( mSurroundings[TC], block->tilePosition.y, block->terrainIndex() ) &&
								check( mSurroundings[BC], block->tilePosition.y, block->terrainIndex() );
	md.br					=	check( mSurroundings[BR], block->tilePosition.y, block->terrainIndex() );
	md.bc					=	check( mSurroundings[BC], block->tilePosition.y, block->terrainIndex() );
	md.bl					=	check( mSurroundings[BL], block->tilePosition.y, block->terrainIndex() );
	md.mr					=	check( mSurroundings[MR], block->tilePosition.y, block->terrainIndex() );
	md.ml					=	check( mSurroundings[ML], block->tilePosition.y, block->terrainIndex() );
	md.tr					=	check( mSurroundings[TR], block->tilePosition.y, block->terrainIndex() );
	md.tc					=	check( mSurroundings[TC], block->tilePosition.y, block->terrainIndex() );
	md.tl					=	check( mSurroundings[TL], block->tilePosition.y, block->terrainIndex() );
	md.rowl					=	check( mSurroundings[BL], block->tilePosition.y, block->terrainIndex() ) &&
								check( mSurroundings[ML], block->tilePosition.y, block->terrainIndex() ) &&
								check( mSurroundings[TL], block->tilePosition.y, block->terrainIndex() );
	md.rowr					=	check( mSurroundings[BR], block->tilePosition.y, block->terrainIndex() ) &&
								check( mSurroundings[MR], block->tilePosition.y, block->terrainIndex() ) &&
								check( mSurroundings[TR], block->tilePosition.y, block->terrainIndex() );
	md.rowt					=	check( mSurroundings[TL], block->tilePosition.y, block->terrainIndex() ) &&
								check( mSurroundings[TC], block->tilePosition.y, block->terrainIndex() ) &&
								check( mSurroundings[TR], block->tilePosition.y, block->terrainIndex() );
	md.rowb					=	check( mSurroundings[BL], block->tilePosition.y, block->terrainIndex() ) &&
								check( mSurroundings[BC], block->tilePosition.y, block->terrainIndex() ) &&
								check( mSurroundings[BR], block->tilePosition.y, block->terrainIndex() );
	
	BlockMeshType newMeshType = BlockMeshNone;
	float rotationY = 0.0f;
	
	
	// Inivisble when completely surrounded and on top
	bool meshIsInvisible = true;
	if ( !check( mCenter, block->tilePosition.y+1, block->terrainIndex() ) ) {
		meshIsInvisible = false;
	}
	for( std::vector<Tile*>::iterator iter = mSurroundings.begin(); iter != mSurroundings.end(); iter++ ) {
		if ( !check( *iter, block->tilePosition.y+1, block->terrainIndex() ) ) {
			meshIsInvisible = false;
		}
	}
	if ( meshIsInvisible ) {
		newMeshType = BlockMeshInvisible;
		rotationY = 0.0f;
	}
	
	// Solid cube when surrounded
	else if ( md.rowt && md.horizontal && md.rowb && md.rowr && md.rowl ) {
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
	
	profile.type = newMeshType;
	profile.rotation = rotationY;
	
	return profile;
}