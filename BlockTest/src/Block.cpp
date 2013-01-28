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

Block::Block() : mRotationTop( 0.0f ), mRotationStack( 0.0 )
{
	mNode = new Node();
}

Block* Block::createBlock( ci::Vec3i tilePosition )
{
	Block* block								= new Block();
	AssetManager* assetManager					= AssetManager::get();
	block->tilePosition							= tilePosition;
	//block->mTextureOffset						= Vec2i( randInt(0,7), randInt(0,7) );
	block->mTextureOffset						= Vec2i( 0, 0 );
	block->mNode->scale							= Vec3f::one() * kTileSize;
	block->mNode->textures[ MaterialDiffuse ]	= assetManager->getTexture( "textures/texture_tiles.png" );
	block->mStackVboMesh						= assetManager->getVboMesh( "models/cube.obj" );
	block->mTopVboMesh							= assetManager->getVboMesh( "models/cube.obj" );
	block->mNode->position						= tilePosition * kTileSize;
	
	return block;
}

Block::~Block()
{
	delete mNode;
}

void Block::draw( ci::gl::GlslProg* shader )
{
	ci::Matrix44f matrix = mNode->transform();
	matrix.rotate( Vec3f::yAxis() * toRadians( mRotationTop ) );
	shader->uniform( "transformMatrix", matrix );
	shader->uniform( "color", ColorA::white() );
	gl::draw( *mTopVboMesh );
	
	for( int i = 1; i < tilePosition.y+1; i++ ) {
		ci::Matrix44f matrix = mNode->transform();
		matrix.rotate( Vec3f::yAxis() * toRadians( mRotationStack ) );
		matrix.translate( -Vec3i::yAxis() * i * kTileSize );
		shader->uniform( "transformMatrix", matrix );
		shader->uniform( "color", ColorA( 1.0f - (float)i / 10.0f, 0.0f, 0.0f, 1.0f ) );
		gl::draw( *mStackVboMesh );
	}
}

void Block::update( const float deltaTime )
{
	mNode->update( deltaTime );
}