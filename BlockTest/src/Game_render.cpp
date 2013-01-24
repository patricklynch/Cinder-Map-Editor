#include "Game.h"

#include "cinder/app/AppBasic.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

void Game::setupRenderer()
{
	try {
		mBlockShader	= GlslInclude::compileShader( "shaders/blocks.vert",	"shaders/blocks.frag" );
		mTerrainShader	= GlslInclude::compileShader( "shaders/terrain.vert",	"shaders/terrain.frag" );
	}
	catch( gl::GlslProgCompileExc &exc ) {
		console() << "Shader compile error: " << exc.what() << std::endl;
		exit( 1 );
	}
	catch( Exception &exc ) {
		console() << "Cannot load shader: " << exc.what() << std::endl;
		exit( 1 );
	}
}

void Game::draw()
{
	gl::clear( kClearColor );
	gl::enableDepthRead();
	gl::enableAlphaBlending();
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	
	for( std::vector<Block*>::const_iterator iter = mBlocks.begin(); iter != mBlocks.end(); iter++ ) {
		Block* block = *iter;
		mBlockShader.bind();
		if ( block->mNode->texture() )
			block->mNode->texture()->bind( 0 );
		mBlockShader.uniform( "texture", 0 );
		mBlockShader.uniform( "transformMatrix",		block->mNode->transform() );
		mBlockShader.uniform( "modelviewMatrix",		mCamera->cinderCamera().getModelViewMatrix() );
		mBlockShader.uniform( "projectionMatrix",		mCamera->cinderCamera().getProjectionMatrix() );
		mBlockShader.uniform( "eyePos",					mCamera->cinderCamera().getEyePoint() );
		mBlockShader.uniform( "lightPos",				mLight.position );
		mBlockShader.uniform( "lightColor",				mLight.color );
		mBlockShader.uniform( "color",					block->mNode->colors[ MaterialDiffuse ] );
		mBlockShader.uniform( "ambientColor",			ColorA( 0.1f, 0.1f, 0.1f, 1.0f ) );
		mBlockShader.uniform( "specularColor",			block->mNode->colors[ MaterialSpecular ] * 0.5f );
		mBlockShader.uniform( "shininess",				10.0f );
		mBlockShader.uniform( "offset",					Vec2f( block->mTextureOffset.x, block->mTextureOffset.y ) );
		mBlockShader.uniform( "numTextures",			(float) kTextureTileSize );
		gl::draw( *block->mNode->mVboMesh );
		if ( block->mNode->texture() )
			block->mNode->texture()->unbind(0);
		mBlockShader.unbind();
	}
	
	Matrix44f matrix = Matrix44f::identity();
	matrix.scale( Vec3f::one() * kTerrainTileMapSize * kTileSize );
	
	for( int pass = 0; pass < 2; pass++ ) {
		if ( pass == 0 ) {
			mTerrainShader.bind();
			mTerrain->mNode->texture()->bind( 0 );
			mTerrain->mTilemapTexture.bind( 1 );
			mTerrain->mTilemapHeightTexture.bind( 2 );
			mTerrainShader.uniform( "transformMatrix",		matrix );
			mTerrainShader.uniform( "modelviewMatrix",		mCamera->cinderCamera().getModelViewMatrix() );
			mTerrainShader.uniform( "projectionMatrix",		mCamera->cinderCamera().getProjectionMatrix() );
			mTerrainShader.uniform( "texture",				0 );
			mTerrainShader.uniform( "tileMap",				1 );
			mTerrainShader.uniform( "heightMap",			2 );
			mTerrainShader.uniform( "color",				ColorA( 1, 1, 1, 1 ) );
			mTerrainShader.uniform( "numTextures",			(float) kTextureTileSize );
			mTerrainShader.uniform( "numTiles",				(float) kTerrainTileMapSize );
			mTerrainShader.uniform( "eyePos",				mCamera->cinderCamera().getEyePoint() );
			mTerrainShader.uniform( "lightPos",				mLight.position );
			mTerrainShader.uniform( "lightColor",			mLight.color );
			mTerrainShader.uniform( "ambientColor",			ColorA( 0.1f, 0.1f, 0.1f, 1.0f ) );
			mTerrainShader.uniform( "specularColor",		mTerrain->mNode->colors[ MaterialSpecular ] * 0.5f );
			mTerrainShader.uniform( "shininess",			0.1f );
		}
		gl::draw( *mTerrain->mNode->mVboMesh );
		mTerrain->mNode->texture()->unbind( 0 );
		mTerrain->mTilemapTexture.unbind(1);
		mTerrain->mTilemapHeightTexture.unbind(2);
		mTerrainShader.unbind();
	}
	
	glDisable( GL_CULL_FACE );
	
	gl::setMatricesWindow( app::getWindowSize(), true );
	// Now draw GUI
}