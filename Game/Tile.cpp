#include "Tile.h"
#include "GameConstants.h"

using namespace ly;

using namespace ci;

Tile::Tile() : mElevation( kMinTileElevation ) {}

Tile::~Tile() {}

void Tile::update( const float deltaTime )
{
	for( std::vector<Block*>::const_iterator iter = mBlocks.begin(); iter != mBlocks.end(); iter++ ) {
		(*iter)->update( deltaTime );
		(*iter)->mTexturePaintMask = mAreaPaintMask;
	}
}

bool Tile::updateBlockProfiles()
{
	bool output = false;
	for( std::vector<Block*>::const_iterator iter = mBlocks.begin(); iter != mBlocks.end(); iter++ ) {
		(*iter)->setProfile( mBlockProfileSelector.getProfile( *iter ) );
	}
	for( int i = 0; i < mBlocks.size(); i++ ) {
		if ( mBlocks[i]->meshType() == BlockMeshNone ) {
			setElevation( mElevation - 1 );
			return true;
		}
	}
	return output;
}

void Tile::draw( ci::gl::GlslProg* shader )
{
	for( std::vector<Block*>::const_iterator iter = mBlocks.begin(); iter != mBlocks.end(); iter++ ) {
		Block* block = *iter;
		block->draw( shader );
	}
}

Block* Tile::getBlockAtElevation( int elevation )
{
	for( std::vector<Block*>::const_iterator iter = mBlocks.begin(); iter != mBlocks.end(); iter++ ) {
		if ( (*iter)->tilePosition.y == elevation ) {
			return *iter;
		}
	}
	return NULL;
}

void Tile::setSurrounding( std::vector<Tile*>& surroundingTiles )
{
	mBlockProfileSelector.setSurrounding( surroundingTiles, this );
}

void Tile::setTilePosition( ci::Vec2i position )
{
	mTilePosition = position;
	for( std::vector<Block*>::const_iterator iter = mBlocks.begin(); iter != mBlocks.end(); iter++ ) {
		Block* block = *iter;		block->setTilePosition( Vec3i( mTilePosition.x, block->tilePosition.y, mTilePosition.y ) );
	}
}

void Tile::setElevation( int elevation )
{
	int difference = elevation - mElevation;
	if ( difference > 0 ) {
		for ( int i = 1; i <= difference; i++ ) {
			Block* block = new Block();
			block->setTilePosition( Vec3i( mTilePosition.x, mElevation + i, mTilePosition.y ) );
			mBlocks.push_back( block );
		}
	}
	else if ( difference < 0 && mBlocks.size() > 1 ) {
		for ( int i = 0; i < math<int>::abs( difference ); i++ ) {
			Block* topBlock = *(mBlocks.end()-1);
			delete topBlock;
			mBlocks.erase( mBlocks.end()-1 );
		}
	}
	mElevation = elevation;
}