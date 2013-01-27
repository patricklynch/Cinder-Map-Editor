#pragma once

namespace ly {

typedef enum {
	ModeObjectSelection,
	ModeTerrainPaint,
} EditorMode;

typedef enum {
	BlockMeshCenter,
	BlockMeshEdgeWall,
	BlockMeshEdgeCenter,
	BlockMeshEdgeOuterCorner
} BlockMeshType;
	
}