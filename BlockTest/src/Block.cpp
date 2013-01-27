#include "Block.h"
#include "GameConstants.h"
#include "Terrain.h"
#include "AssetManager.h"

#include "cinder/app/App.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;

using namespace ly;

Block::Block()
{
	mNode = new Node();
}

Block* Block::createBlock( ci::Vec3i tilePosition )
{
	Block* block = new Block();
	AssetManager* assetManager = AssetManager::get();
	block->tilePosition = tilePosition;
	//block->mTextureOffset = Vec2i( randInt(0,7), randInt(0,7) );
	block->mTextureOffset = Vec2i( 0, 0 );
	block->mNode->scale = Vec3f::one() * kTileSize;
	block->mNode->setTexture( assetManager->getTexture( "textures/texture_tiles.png" ) );
	block->mNode->mVboMesh = assetManager->getVboMesh( "models/cube.obj" );
	block->mStackVboMesh = AssetManager::get()->getVboMesh( "models/cube.obj" );
	block->mNode->mMesh = assetManager->getMesh( "models/cube.obj" );
	block->mNode->position = tilePosition * kTileSize;
	return block;
}

Block::~Block()
{
	delete mNode;
}

void Block::draw( ci::gl::GlslProg* shader )
{
	shader->uniform( "transformMatrix", mNode->transform());
	gl::draw( *mNode->mVboMesh );
	for( int i = 1; i < tilePosition.y+1; i++ ) {
		ci::Matrix44f matrix = mNode->transform();
		matrix.translate( -Vec3i::yAxis() * i * kTileSize );
		shader->uniform( "transformMatrix", matrix );
		gl::draw( *mStackVboMesh );
	}
}

void Block::update( const float deltaTime )
{
	mNode->update( deltaTime );
}