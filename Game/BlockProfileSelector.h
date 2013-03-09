#pragma once

#include <vector>

#include "Block.h"
#include "cinder/Vector.h"

namespace ly {

class Tile;
	
struct SurroundingBlockData {
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
} SurroundingBlockType;

class BlockProfileSelector {
public:
	BlockProfileSelector();
	virtual ~BlockProfileSelector();
	
	void									setSurrounding( std::vector<Tile*>& selections, Tile* centerTile );
	BlockProfile							getProfile( Block* block );
	bool									needsUpdate();
	
	const std::vector<Tile*>				surroundings() const { return mSurroundings; };
	bool									check( Tile* surrounding, int elevation, int terrainIndex );
	
private:
	SurroundingBlockData					mSurroundingBlockData;
	std::vector<Tile*>						mSurroundings;
	Tile*									mCenter;
};

}