#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Surface.h"

#include "Node.h"

namespace ly {

class Block {
public:
	static Block*			createBlock( ci::Vec3i tilePosition );
	virtual					~Block();
	
	void					update( const float deltaTime );
	void					draw( ci::gl::GlslProg* shader );
	ci::Vec2i				mTextureOffset;
	Node*					mNode;
	ci::Vec3i				tilePosition;
	ci::gl::VboMesh*		mStackVboMesh;
	
	bool					operator==( Block* other ) { return other->tilePosition == tilePosition; }
	
private:
	Block();
};
	
}