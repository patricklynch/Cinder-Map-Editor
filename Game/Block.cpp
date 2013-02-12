#include "Block.h"
#include "GameConstants.h"
#include "Terrain.h"
#include "AssetManager.h"
#include "Game.h"

#include "cinder/CinderMath.h"
#include "cinder/app/App.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;

using namespace ly;

Block::Block( Game* game ) : mRotation( 0.0f ), mTerrain( NULL ), mGame( game )
{
	mNode = new Node();
	testOffset = Vec2f::zero();
	mNode->colors[ MaterialSpecular ] = ColorA( 0, 0, 0, 0 );
	
	AssetManager* assetManager	= AssetManager::get();
	tilePosition				= Vec3f::zero();
	mTextureOffset				= Vec2i( 2, 3 );
	//mTextureOffset			= Vec2i( randInt(0,7), randInt(0,7) );
	mNode->scale				= Vec3f::one() * kTileSize;

	setTerrainIndex( 0 );
	
	mBlankMask					= assetManager->getTexture( "textures/default_black.png" );
	mTexturePaintMask			= NULL;
}

void Block::setTerrainIndex( int index )
{
	mTerrainIndex = index;
	mTerrain = mGame->blockTerrains()[ mTerrainIndex ];
}

void Block::setTilePosition( ci::Vec3i newTilePosition )
{
	tilePosition				= newTilePosition;
	mNode->position				= tilePosition * kTileSize;
}

void Block::setMeshType( BlockMeshType type, float rotation )
{
	mBlockMeshType = type;

	mRotation = rotation;
	float textureRotation = rotation;

	// TODO: Is there a baetter way to do this?
	if ( mBlockMeshType == BlockMeshCenter ) {
		mRotation = 0.0f;
		textureRotation = 180.0f;
	}
	
	//mTextureMatrix = Matrix44f::identity();
	//mTextureMatrix.translate( Vec3f( 0.5f, 0.5f, 0.5f ) );
	//mTextureMatrix.rotate( Vec3f::zAxis(), toRadians( textureRotation ) );
	//mTextureMatrix.translate( Vec3f( -0.5f, -0.5f, 0.5f ) );
	//mTextureMatrix.scale( Vec3f( -1.0f, 1.0f, 0.0f ) );
	
	mTextureMatrix = Matrix44f::identity();
	mTextureMatrix.translate( Vec3f( 0.5f, 0.5f, 0.5f ) );
	mTextureMatrix.rotate( Vec3f::zAxis(), toRadians( textureRotation ) );
	mTextureMatrix.translate( Vec3f( -0.5f, -0.5f, 0.5f ) );
}

Block::~Block()
{
	delete mNode;
}

void Block::draw( ci::gl::GlslProg* shader )
{
	if ( !mTerrain || mBlockMeshType == BlockMeshNone ) return;
	
	gl::enableDepthRead();
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	
	ci::gl::Texture* currentMask;
	if ( meshType() == BlockMeshCenter ) {
		currentMask = mBlankMask;
	} else {
		currentMask = mTerrain->mMaskTexture;
	}
	
	ci::gl::Texture* currentPaintMask;
	if ( !mTexturePaintMask ) {
		currentPaintMask = mBlankMask;
	} else {
		currentPaintMask = mTexturePaintMask;
	}
	
	bool drawEdge = false;
	
	currentMask->bind( 0 );
	mGame->blockTextures()[0]->bind( 1 );
	mGame->blockTextures()[1]->bind( 2 );
	mGame->blockTextures()[2]->bind( 3 );
	mGame->blockTextures()[3]->bind( 4 );
	mTerrain->mTexture->bind(5);
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
	//shader->uniform( "textureMatrix2",			mTextureMatrix2 );
	
	float r = kDefaultMaxVisibleTileRadius;
	shader->uniform( "offset", Vec2f( tilePosition.x, tilePosition.z ) + Vec2f( r, r ) );
	
	gl::disableDepthWrite();
	gl::draw( *mTerrain->mTileMeshes[ mBlockMeshType ] );
	gl::enableDepthWrite();
	
	drawEdge = meshType() == BlockMeshCenter ? false : true;
	shader->uniform( "edge", drawEdge );
	gl::draw( *mTerrain->mEdgeMeshes[ mBlockMeshType ] );
	
	currentMask->unbind(0);
	mGame->blockTextures()[0]->unbind(1);
	mGame->blockTextures()[1]->unbind(2);
	mGame->blockTextures()[2]->unbind(3);
	mGame->blockTextures()[3]->unbind(4);
	mTerrain->mTexture->unbind(5);
	currentPaintMask->unbind(6);
}

void Block::update( const float deltaTime )
{
	mNode->rotation.y = mRotation;
	mNode->update( deltaTime );
	
	testOffset += Vec2f::one() * 0.01f;
}