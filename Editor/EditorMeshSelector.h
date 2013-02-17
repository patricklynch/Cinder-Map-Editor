#pragma once

#include <vector>

#include "Block.h"
#include "cinder/Vector.h"

namespace ly {

class EditorSelection;
	
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

struct EditorMeshSelectorResult {
	float rotation;
	BlockMeshType type;
};

class EditorMeshSelector {
public:
	EditorMeshSelector();
	virtual ~EditorMeshSelector();
	
	void									setSurrounding( std::vector<EditorSelection*>& selections, ci::Vec3i centerTilePosition  );
	EditorMeshSelectorResult				getMeshSelection( Block* block );
	
	const std::vector<EditorSelection*>		surroundings() const { return mSurroundings; };
	bool									check( EditorSelection* surrounding, Block* block );
	
private:
	MeshPositionData						mMeshPositionData;
	std::vector<EditorSelection*>			mSurroundings;
};

}