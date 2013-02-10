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

Block::Block( ci::Vec3i atTilePosition ) : mRotation( 0.0f )
{
	mNode = new Node();
	testOffset = Vec2f::zero();
	mNode->colors[ MaterialSpecular ] = ColorA( 0, 0, 0, 0 );
	
	AssetManager* assetManager	= AssetManager::get();
	tilePosition				= atTilePosition;
	mTextureOffset				= Vec2i( 2, 3 );
	//mTextureOffset			= Vec2i( randInt(0,7), randInt(0,7) );
	mNode->scale				= Vec3f::one() * kTileSize;
	mNode->position				= tilePosition * kTileSize;
	
	mTextureD					= assetManager->getTexture( "textures/grass.png" );
	mTextureR					= assetManager->getTexture( "textures/stone.png" );
	mTextureG					= assetManager->getTexture( "textures/concrete.png" );
	mTextureB					= assetManager->getTexture( "textures/brick.png" );
	
	mTextureEdge				= assetManager->getTexture( "textures/dirtwall.png" );
	
	mBlankMask					= assetManager->getTexture( "textures/default_black.png" );
	mTextureMask				= assetManager->getTexture( "terrain/outcropping/mask.png" );
	mTexturePaintMask			= NULL;
	//mTexturePaintMask			= assetManager->getTexture( "textures/test_paint_mask.png" );
}

void Block::setMeshType( BlockMeshType type, float rotation )
{
	// TODO: Fix up this rotation issue in the models
	mRotation = rotation;
	float textureRotation = 180.0f - mRotation;
	
	AssetManager* assetManager = AssetManager::get();
	mBlockMeshType = type;
	switch( mBlockMeshType ) {
		case BlockMeshFill:
			mRotation = 0.0f;
			mNode->mVboMesh		= assetManager->getVboMesh( "terrain/outcropping/edge/center.obj" );
			mVboMeshTile		= assetManager->getVboMesh( "terrain/outcropping/tile/center.obj" );
			break;
		case BlockMeshEdge:
			mNode->mVboMesh		= assetManager->getVboMesh( "terrain/outcropping/edge/wall.obj" );
			mVboMeshTile		= assetManager->getVboMesh( "terrain/outcropping/tile/wall.obj" );
			break;
		case BlockMeshOuterCorner:
			mNode->mVboMesh		= assetManager->getVboMesh( "terrain/outcropping/edge/corner.obj" );
			mVboMeshTile		= assetManager->getVboMesh( "terrain/outcropping/tile/corner.obj" );
			break;
		case BlockMeshInnerCorner:
			mNode->mVboMesh		= assetManager->getVboMesh( "terrain/outcropping/edge/inner_corner.obj" );
			mVboMeshTile		= assetManager->getVboMesh( "terrain/outcropping/tile/inner_corner.obj" );
			break;
		case BlockMeshDoubleInnerCorner:
			mNode->mVboMesh		= assetManager->getVboMesh( "terrain/outcropping/edge/bridge.obj" );
			mVboMeshTile		= assetManager->getVboMesh( "terrain/outcropping/tile/bridge.obj" );
			break;
		case BlockMeshNone:
			mNode->mVboMesh		= NULL;
			mVboMeshTile		= NULL;
			break;
		default:
			break;
	}
	
	mTextureMatrix = Matrix44f::identity();
	mTextureMatrix.translate( Vec3f( 0.5f, 0.5f, 0.5f ) );
	mTextureMatrix.rotate( Vec3f::zAxis(), toRadians( textureRotation ) );
	mTextureMatrix.translate( Vec3f( -0.5f, -0.5f, 0.5f ) );
	mTextureMatrix.scale( Vec3f( -1.0f, 1.0f, 0.0f ) );
	
	mTextureMatrix2 = Matrix44f::identity();
	mTextureMatrix2.translate( Vec3f( 0.5f, 0.5f, 0.5f ) );
	mTextureMatrix2.rotate( Vec3f::zAxis(), toRadians( -mRotation ) );
	mTextureMatrix2.translate( Vec3f( -0.5f, -0.5f, 0.5f ) );
}

Block::~Block()
{
	delete mNode;
}

void Block::draw( ci::gl::GlslProg* shader )
{
	if ( !mNode->mVboMesh || !mVboMeshTile ) return;
	
	gl::enableDepthRead();
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	
	ci::gl::Texture* currentMask;
	if ( meshType() == BlockMeshFill ) {
		currentMask = mBlankMask;
	} else {
		currentMask = mTextureMask;
	}
	
	ci::gl::Texture* currentPaintMask;
	if ( !mTexturePaintMask ) {
		currentPaintMask = mBlankMask;
	} else {
		currentPaintMask = mTexturePaintMask;
	}
	
	bool drawEdge = false;
	
	currentMask->bind( 0 );
	mTextureD->bind( 1 );
	mTextureR->bind( 2 );
	mTextureG->bind( 3 );
	mTextureB->bind( 4 );
	mTextureEdge->bind(5);
	currentPaintMask->bind(6);
	
	shader->uniform( "transformMatrix",			mNode->transform() );
	shader->uniform( "color",					mNode->colors[ MaterialDiffuse ] );
	shader->uniform( "specularColor",			mNode->colors[ MaterialSpecular ] * 0.5f );
	shader->uniform( "shininess",				50.0f );
	shader->uniform( "offset",					Vec2f( mTextureOffset.x, mTextureOffset.y ) );
	shader->uniform( "edge",					drawEdge );
	shader->uniform( "numTextures",				kDefaultMaxVisibleTileRadius * 2.0f + 1.0f );
	shader->uniform( "textureMask",				0 );
	shader->uniform( "textureD",				1 );
	shader->uniform( "textureR",				2 );
	shader->uniform( "textureG",				3 );
	shader->uniform( "textureB",				4 );
	shader->uniform( "textureEdge",				5 );
	shader->uniform( "texturePaintMask",		6 );
	shader->uniform( "textureMatrix",			mTextureMatrix );
	shader->uniform( "textureMatrix2",			mTextureMatrix2 );
	
	float r = kDefaultMaxVisibleTileRadius;
	shader->uniform( "offset", Vec2f( tilePosition.x, tilePosition.z ) + Vec2f( r, r ) );
	
	gl::disableDepthWrite();
	gl::draw( *mVboMeshTile );
	gl::enableDepthWrite();
	
	drawEdge = meshType() == BlockMeshFill ? false : true;
	shader->uniform( "edge", drawEdge );
	gl::draw( *mNode->mVboMesh );
	
	currentMask->unbind(0);
	mTextureD->unbind(1);
	mTextureR->unbind(2);
	mTextureG->unbind(3);
	mTextureB->unbind(4);
	mTextureEdge->unbind(5);
	currentPaintMask->unbind(6);
}

void Block::update( const float deltaTime )
{
	mNode->rotation.y = mRotation;
	mNode->update( deltaTime );
	
	testOffset += Vec2f::one() * 0.01f;
}