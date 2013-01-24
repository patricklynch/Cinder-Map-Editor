#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/ImageIo.h"
#include "cinder/params/Params.h"
#include "cinder/Rand.h"
#include "cinder/gl/Vbo.h"
#include "cinder/Surface.h"

#include "MeshHelper.h"
#include "Camera.h"
#include "Input.h"
#include "GlslInclude.h"
#include "EditorCamera.h"
#include "Block.h"
#include "AssetManager.h"
#include "Resources.h"
#include "Constants.h"
#include "GameConstants.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace ly {
	
struct SimpleLight {
	ci::Vec3f position;
	ci::ColorA color;
};

class BlockTestApp : public AppBasic {
  public:
	void prepareSettings( Settings *settings );
	void setup();
	
	void mouseDown( ci::app::MouseEvent event ) { ly::Input::get()->mouseDown( event ); }
	void mouseMove( ci::app::MouseEvent event ) { ly::Input::get()->mouseMove( event ); }
	void mouseDrag( ci::app::MouseEvent event ) { ly::Input::get()->mouseDrag( event ); }
	void mouseUp  ( ci::app::MouseEvent event ) { ly::Input::get()->mouseUp( event ); }
	void keyDown  ( ci::app::KeyEvent event )   {ly:: Input::get()->keyDown  ( event ); }
	
	void resize( ResizeEvent event );
	void update();
	void draw();
	
	SimpleLight				mLight;
	ly::Camera*				mCamera;
	float					mPrevElapsedSeconds;
	ly::EditorCamera		mEditorCamera;
	gl::GlslProg			mBlockShader;
	gl::GlslProg			mTerrainShader;
	std::vector<Block*>		mBlocks;
	Terrain*				mTerrain;
};
	
}

using namespace ly;

void BlockTestApp::prepareSettings( Settings *settings )
{
	settings->setFrameRate( kFrameRate );
	settings->setWindowSize( kWindowWidth, kWindowHeight );
}

void BlockTestApp::setup()
{
	// Preload assets
	AssetManager* assetManager = AssetManager::get();
	std::string assets[] = {
		"shaders/blocks.vert",
		"shaders/blocks.frag",
		"shaders/terrain.vert",
		"shaders/terrain.frag",
		"models/yoda.obj",
		"models/cube_multiface.obj",
		"models/cube_smooth_multiface.obj",
		"models/cube_smooth.obj",
		"models/cube.obj",
		"models/cylinder.obj",
		"models/dome_tall.obj",
		"models/dome.obj",
		"models/geosphere_center.obj",
		"models/geosphere.obj",
		"models/tri_prism.obj",
		"models/plane.obj",
		"textures/texture_tiles.png"
	};
	assetManager->loadAssets( &assets[0], sizeof( assets ) / sizeof( std::string ) );
	
	mCamera = ly::Camera::get();
	mCamera->setFov( 45 );
	mCamera->setZoom( 3 );
	mCamera->rotation.y = 45.0f;
	mCamera->setAngle( -45.0f);
	
	mLight.position = Vec3f( 20, 100, 30 );
	mLight.color = ColorA::white();
	
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
	
	mTerrain = new Terrain();
	mTerrain->mNode->setTexture( assetManager->getTexture( "textures/texture_tiles.png" ) );
	mTerrain->mNode->mVboMesh = assetManager->getVboMesh( "models/plane.obj" );
	
	{	// Create Blocks
		gl::Texture* texture = assetManager->getTexture( "textures/texture_tiles.png" );
		int n = 10;
		int i = 0;
		for(int x = -n; x < n; x++) {
			for(int z = -n; z < n; z++) {
				if ( randBool() ) continue;
				Block* block = new Block();
				block->mTextureOffset = Vec2i( randInt(0,7), randInt(0,7) );
				block->mNode->scale = Vec3f::one() * kTileSize;
				block->mNode->setTexture( texture );
				block->mNode->mVboMesh = assetManager->getVboMesh( "models/dome.obj" );
				int y = 0;
				block->mNode->position = Vec3f( x, y, z ) * kTileSize;
				mBlocks.push_back( block );
			}
		}
	}
}

void BlockTestApp::resize( ResizeEvent event )
{
}

void BlockTestApp::update()
{
    float currentTime = (float) app::getElapsedSeconds();
    float deltaTime = currentTime - mPrevElapsedSeconds;
    mPrevElapsedSeconds = currentTime;
	
	mCamera->update( deltaTime);
	mEditorCamera.update( deltaTime );
	ly::Input::get()->update( deltaTime );
	
	for( std::vector<Block*>::const_iterator iter = mBlocks.begin(); iter != mBlocks.end(); iter++ ) {
		Block* block = *iter;
		block->update( deltaTime );
	}
	mTerrain->update( deltaTime );
}

void BlockTestApp::draw()
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
			mTerrainShader.uniform( "specularColor",		ColorA::white() * 0.5f );
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


CINDER_APP_BASIC( BlockTestApp, RendererGl(0) )
