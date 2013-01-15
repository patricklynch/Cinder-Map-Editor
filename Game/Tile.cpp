#include "Tile.h"
#include "Renderer.h"
#include "AssetManager.h"

#include "cinder/Rand.h"
#include "cinder/app/App.h"
#include "cinder/gl/Texture.h"

using namespace ly;

using namespace ci;

Tile::Tile( int startX, int startY )
{
	x = startX;
	y = startY;
	
	gl::Texture& texture1 = AssetManager::get()->getTexture( "texture_tiles.png" );
	
	mNode = new Node( NodeTypeCube );
	mNode->size = Vec3f( kTileSize, kTileSize, kTileSize );
	mNode->setTexture( &texture1 );
	mNode->anchor = Vec3f( 0.5f, 0.0f, 0.5f );
	ly::Renderer::get()->addNode( mNode );
}

Tile::~Tile()
{
	ly::Renderer::get()->removeNode( mNode );
}

void Tile::update( const float deltaTime )
{
	Vec3f pos = realPosition( Vec2i( x, y ) );
	mNode->position = Vec3f( pos.x, pos.y, pos.z );
	mNode->update( deltaTime );
}

Vec2i Tile::tilePosition( Vec3f realPosition )
{
	float dx = realPosition.x < 0.0f ? -1.0f : 1.0f;
	float dy = realPosition.z < 0.0f ? -1.0f : 1.0f;
	return Vec2i( realPosition.x / kTileSize + kTileSize * 0.5f * dx,
				  realPosition.z / kTileSize + kTileSize * 0.5f * dy);
}

Vec3f Tile::realPosition( Vec2i tilePosition )
{
	return Vec3f( tilePosition.x * kTileSize, 0.0f, tilePosition.y * kTileSize );
}