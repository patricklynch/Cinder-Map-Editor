#include "Block.h"

#include "cinder/app/App.h"
#include "cinder/Rand.h"

#include "MeshHelper.h"
#include "GameConstants.h"

using namespace ci;
using namespace ci::app;

using namespace ly;

Block::Block()
{
	mNode = new Node();
}

Block::~Block()
{
	delete mNode;
}

void Block::update( const float deltaTime )
{
	mNode->update( deltaTime );
}

Terrain::Terrain()
{
	mNode = new Node();
	
	gl::Texture::Format format;
	format.enableMipmapping();
	format.setMagFilter( GL_NEAREST );
	
	gl::Texture::Format tilemapFormat;
	tilemapFormat.enableMipmapping();
	tilemapFormat.setMagFilter( GL_NEAREST );
	
	mTilemapSurface = Surface8u( kTerrainTileMapSize, kTerrainTileMapSize, false );
	for( int y = 0; y < mTilemapSurface.getHeight(); y++) {
		for( int x = 0; x < mTilemapSurface.getWidth(); x++) {
			ColorA color( (float) randInt(0,kTextureTileSize) / (float) kTextureTileSize, (float) randInt(0,kTextureTileSize) / (float) kTextureTileSize, 0.0f, 0.0f );
			mTilemapSurface.setPixel( Vec2i( x, y ), color );
		}
	}
	mTilemapTexture = gl::Texture( mTilemapSurface, format );
	
	mTilemapHeightSurface = Surface8u( kTerrainTileMapSize+1, kTerrainTileMapSize+1, false );
	for( int y = 0; y < mTilemapHeightSurface.getHeight(); y++) {
		for( int x = 0; x < mTilemapHeightSurface.getWidth(); x++) {
			ColorA color( 1.0f, randFloat() * 0.2f, 1.0f, 1.0f );
			mTilemapHeightSurface.setPixel( Vec2i( x, y ), color );
		}
	}
	mTilemapHeightTexture = gl::Texture( mTilemapHeightSurface, tilemapFormat );
}

Terrain::~Terrain()
{
	delete mNode;
}


void Terrain::update( const float deltaTime )
{
	mNode->update( deltaTime );
}
