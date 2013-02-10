#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"
#include "cinder/Surface.h"

#include "Node.h"

#include <vector>

namespace ly {
	
class Tile /*: DataObject*/ {
public:
	int x;
	int y;
	int offsetX;
	int offsetY;
};

class Terrain {
public:
	Terrain();
	virtual					~Terrain();
	void					update( const float deltaTime );
	
	static ci::Vec3i		mapLocation( ci::Vec3f realPosition );
	static ci::Vec3f		realPosition( ci::Vec3i tilePosition );
	
	ci::Vec2i				mTextureOffset;
	Node*					mNode;
	ci::Vec2i				mapLocationCenter;
	
	ci::Surface8u			mTilemapSurface;
	ci::gl::Texture			mTilemapTexture;
	
	void					setTiles( std::vector<Tile*> tiles );
	
private:
	std::vector<Tile*>		mLoadedTiles;
	
};

}