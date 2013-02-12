#pragma once

#include "cinder/gl/Texture.h"
#include "cinder/gl/Vbo.h"

#include <map>
#include <string>

namespace ly {

typedef enum {
	BlockMeshNone = -1,
	BlockMeshCenter,
	BlockMeshWall,
	BlockMeshCorner,
	BlockMeshInnerCorner,
	BlockMeshBridge
} BlockMeshType;

class Terrain {
public:
	Terrain( std::string name );
	Terrain();
	~Terrain();

	void load( std::string name );

	std::string name;
	std::map<BlockMeshType, ci::gl::VboMesh*> mEdgeMeshes;
	std::map<BlockMeshType, ci::gl::VboMesh*> mTileMeshes;
	ci::gl::Texture* mMaskTexture;
	ci::gl::Texture* mTexture;
};

}