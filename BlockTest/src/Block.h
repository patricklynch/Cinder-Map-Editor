#pragma once

#include "cinder/Vector.h"
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
	ci::Vec3i				tilePosition;
	
};
	
}