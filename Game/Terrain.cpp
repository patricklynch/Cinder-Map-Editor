#include "Terrain.h"
#include "GameConstants.h"

#include "cinder/app/App.h"
#include "cinder/Rand.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

Terrain::Terrain()
{
	// Node object for basic rendering properties
	mNode = new Node();
	mNode->scale = Vec3f::one() * kTerrainTileMapSize * kTileSize;
	
	// Generate some random tiles in the way we expec them to be loaded from the database
	std::vector<Tile*> tiles;
	for( int y = 0; y < kTerrainTileMapSize; y++) {
		for( int x = 0; x < kTerrainTileMapSize; x++) {
			Tile* tile = new Tile();
			tile->x = x;
			tile->y = y;
			tile->offsetX = randInt( 0, kTextureTileSize );
			tile->offsetY = randInt( 0, kTextureTileSize );
			tiles.push_back( tile );
		}
	}
	setTiles( tiles );
}

void Terrain::setTiles( std::vector<Tile*> tiles )
{
	// Cache the tiles in this vector:
	mLoadedTiles = tiles;
	
	// Generator a surface using data from the tiles vector
	mTilemapSurface = Surface8u( kTerrainTileMapSize, kTerrainTileMapSize, false );
	for( std::vector<Tile*>::const_iterator iter = mLoadedTiles.begin(); iter != mLoadedTiles.end(); iter++ ) {
		Tile* tile = *iter;
		ColorA color( (float) tile->offsetX / (float) kTextureTileSize,
					  (float) tile->offsetY / (float) kTextureTileSize,
					  0.0f, 0.0f );
		mTilemapSurface.setPixel( Vec2i( tile->x, tile->y ), color );
	}
	
	// Create a texture from that surface the shader will use to pick subtextures
	gl::Texture::Format format;
	format.enableMipmapping();
	format.setMagFilter( GL_NEAREST );
	mTilemapTexture = gl::Texture( mTilemapSurface, format );
}

Terrain::~Terrain()
{
	delete mNode;
}

void Terrain::update( const float deltaTime )
{
	// Update the node
	//mNode->position = realPosition( mapLocationCenter );
	//mNode->update( deltaTime );
}

Vec3i Terrain::mapLocation( Vec3f realPosition )
{
	float dx = realPosition.x < 0.0f ? -1.0f : 1.0f;
	float dy = realPosition.y < 0.0f ? -1.0f : 1.0f;
	float dz = realPosition.z < 0.0f ? -1.0f : 1.0f;
	return Vec3i(realPosition.x / kTileSize + kTileSize * 0.5f * dx,
				 realPosition.y / kTileSize + kTileSize * 0.5f * dy,
				 realPosition.z / kTileSize + kTileSize * 0.5f * dz);
}

Vec3f Terrain::realPosition( Vec3i tilePosition )
{
	return tilePosition * kTileSize;
}