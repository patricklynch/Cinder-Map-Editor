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
	testOffset = Vec2f::zero();
	mNode->colors[ MaterialSpecular ] = ColorA( 0, 0, 0, 0 );
}

Block* Block::createBlock( ci::Vec3i tilePosition )
{
	AssetManager* assetManager		= AssetManager::get();
	Block* block					= new Block();
	block->tilePosition				= tilePosition;
	block->mTextureOffset			= Vec2i( 2, 3 );
	//block->mTextureOffset			= Vec2i( randInt(0,7), randInt(0,7) );
	block->mNode->scale				= Vec3f::one() * kTileSize;
	block->mNode->position			= tilePosition * kTileSize;
	
	block->mVboMeshTile				= NULL;
	block->mVboMeshEdge				= NULL;
	
	block->mTexture1				= assetManager->getTexture( "textures/grass.png" );
	block->mTexture2				= assetManager->getTexture( "textures/dirtwall.png" );
	block->mBlankMask				= assetManager->getTexture( "textures/default_black.png" );
	block->mTextureMask				= assetManager->getTexture( "terrain/outcropping/mask.png" );
	
	return block;
}

void Block::setMeshType( BlockMeshType type, float rotation )
{
	mRotation = rotation;
	
	AssetManager* assetManager = AssetManager::get();
	mBlockMeshType = type;
	switch( mBlockMeshType ) {
		case BlockMeshFill:
			mNode->mVboMesh = assetManager->getVboMesh( "terrain/outcropping/center.obj" );
			break;
		case BlockMeshEdge:
			mNode->mVboMesh = assetManager->getVboMesh( "terrain/outcropping/wall.obj" );
			break;
		case BlockMeshOuterCorner:
			mNode->mVboMesh = assetManager->getVboMesh( "terrain/outcropping/corner.obj" );
			break;
		case BlockMeshInnerCorner:
			mNode->mVboMesh = assetManager->getVboMesh( "terrain/outcropping/inner_corner.obj" );
			break;
		case BlockMeshDoubleInnerCorner:
			mNode->mVboMesh = assetManager->getVboMesh( "terrain/outcropping/bridge.obj" );
			break;
		case BlockMeshNone:
			mNode->mVboMesh = NULL;
			break;
		default:
			break;
	}
}

Block::~Block()
{
	delete mNode;
}

void Block::draw( ci::gl::GlslProg* shader )
{
	if ( !mNode->mVboMesh ) return;
	
	gl::enableAlphaBlending();
	
	ci::gl::Texture* currentMask;
	if ( meshType() == BlockMeshFill ) {
		currentMask = mBlankMask;
	} else {
		currentMask = mTextureMask;
	}
	
	currentMask->bind( 0 );
	mTexture1->bind( 1 );
	mTexture2->bind( 2 );
	
	shader->uniform( "textureMask", 0 );
	shader->uniform( "texture1", 1 );
	shader->uniform( "texture2", 2 );
	
	shader->uniform( "testOffset", testOffset );
	
	gl::draw( *mNode->mVboMesh );
	
	currentMask->unbind(0);
	mTexture1->unbind(1);
	mTexture2->unbind(2);
}

void Block::update( const float deltaTime )
{
	mNode->rotation.y = mRotation;
	mNode->update( deltaTime );
	
	testOffset += Vec2f::one() * 0.01f;
}