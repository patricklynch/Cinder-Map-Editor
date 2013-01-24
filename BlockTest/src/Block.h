#pragma once

#include "cinder/Vector.h"
#include "cinder/TriMesh.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Surface.h"

#include "Node.h"

namespace ly {

class Block {
public:
							Block();
	virtual					~Block();
	void					update( const float deltaTime );
	
	ci::Vec2i				mTextureOffset;
	Node*					mNode;
	
private:
	
};
	
class Terrain {
public:
							Terrain();
	virtual					~Terrain();
	void					update( const float deltaTime );
	
	ci::Vec2i				mTextureOffset;
	Node*					mNode;
	
	ci::Surface8u			mTilemapSurface;
	ci::gl::Texture			mTilemapTexture;
	ci::Surface8u			mTilemapHeightSurface;
	ci::gl::Texture			mTilemapHeightTexture;
};

}