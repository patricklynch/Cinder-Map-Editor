#include "Block.h"
#include "GameConstants.h"
#include "Terrain.h"
#include "AssetManager.h"

#include "cinder/CinderMath.h"
#include "cinder/app/App.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;

using namespace ly;

Block::Block() : mRotation( 0.0f )
{
	mNode = new Node();
}

Block* Block::createBlock( ci::Vec3i tilePosition )
{
	AssetManager* assetManager					= AssetManager::get();
	Block* block								= new Block();
	block->tilePosition							= tilePosition;
	block->mTextureOffset						= Vec2i( 2, 3 );
	//block->mTextureOffset						= Vec2i( randInt(0,7), randInt(0,7) );
	block->mNode->scale							= Vec3f::one() * kTileSize;
	block->mNode->textures[ MaterialDiffuse ]	= assetManager->getTexture( "textures/texture_tiles.png" );
	//block->mNode->colors[ MaterialDiffuse ]		= ColorA(1,0,0,1);
	block->mNode->position						= tilePosition * kTileSize;
	
	return block;
}

Block::~Block()
{
	delete mNode;
}

void Block::draw( ci::gl::GlslProg* shader )
{
	if ( mNode->mVboMesh )
		gl::draw( *mNode->mVboMesh );
}

void Block::update( const float deltaTime )
{
	mNode->rotation.y = mRotation;
	mNode->update( deltaTime );
}