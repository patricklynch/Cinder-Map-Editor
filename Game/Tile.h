#pragma once

#include "Block.h"
#include "BlockProfileSelector.h"

#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Vector.h"

#include <vector>

namespace ly {

class Tile {
public:
	Tile();
	virtual ~Tile();
	
	void					update( const float deltaTime = 0.0f );
	void					draw( ci::gl::GlslProg* shader );
	
	std::vector<Block*>&	blocks() { return mBlocks; }
	void					setElevation( int elevation );
	void					setTilePosition( ci::Vec2i position );
	void					setSurrounding( std::vector<Tile*>& surroundingTiles );
	void					setPaintMask( ci::gl::Texture* paintMask ) { mAreaPaintMask = paintMask; }
	
	int						getTerrainIndex() const { return 0; }
	int						getElevation() const { return mElevation; }
	ci::Vec2i				getTilePosition() const { return mTilePosition; }
	ci::Vec3i				getWorldPosition() const { return ci::Vec3i( mTilePosition.x, mElevation, mTilePosition.y ); }
	Block*					getBlockAtElevation( int elevation );
	bool					updateBlockProfiles();
	
private:
	ci::gl::Texture*		mAreaPaintMask;
	int						mElevation;
	ci::Vec2i				mTilePosition;
	std::vector<Block*>		mBlocks;
	std::vector<Tile*>		mSurroundingTiles;
	BlockProfileSelector	mBlockProfileSelector;
};

}