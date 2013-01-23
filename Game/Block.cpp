#include "Block.h"
#include "Renderer.h"
#include "GameConstants.h"
#include "AssetManager.h"

#include "cinder/gl/Texture.h"

using namespace ly;

using namespace ci;

Block::Block( ci::Vec3i blockPos )
{
	mRoot = new Node();
	mRoot->position = blockPos * kTileSize;
	ly::Renderer::get()->addNode( mRoot );
	
	
	gl::Texture* texture1 = AssetManager::get()->getTexture( "texture_tiles.png" );
	
	for( int x = 0; x <= 1; x++ ) {
		for( int y = 0; y <= 1; y++ ) {
			for( int z = 0; z <= 1; z++ ) {
				if ( x == 0 && y == 0 && z == 0 ) continue;
				Node* node = new Node();
				node->size = Vec3f( kTileSize, kTileSize, kTileSize );
				node->position = Vec3f( x, y, z ) * kTileSize * 0.5f;
				node->parent = mRoot;
				node->setTexture( texture1 );
				ly::Renderer::get()->addNode( node );
				mNodes.push_back( node );
			}
		}
	}
}

Block::~Block()
{
	for( std::vector<Node*>::iterator iter = mNodes.begin(); iter != mNodes.end(); iter++ ) {
		ly::Renderer::get()->removeNode( *iter );
		delete *iter;
	}
	delete mRoot;
}

void Block::update( const float deltaTime )
{
	mRoot->update( deltaTime );
	for( std::vector<Node*>::iterator iter = mNodes.begin(); iter != mNodes.end(); iter++ )
		(*iter)->update( deltaTime );
}