#include "Game.h"

#include "cinder/app/AppBasic.h"
#include "cinder/params/Params.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

// DEBUG params
float mTerrainScaleAdjust;
float mTerrainOffsetAdjust;

void Game::setupRenderer()
{
	try {
		mBlockShader	= GlslInclude::compileShader( "shaders/blocks.vert",	"shaders/blocks2.frag" );
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
	
	/*mParams = params::InterfaceGl( "DEBUG", Vec2i( 200, 300 ) );
	mTerrainScaleAdjust = 1.0f;
	mParams.addParam( "Terrain Scale Adjust", &mTerrainScaleAdjust, "max=2.0 min=0.0 step=0.00001" );
	mTerrainOffsetAdjust = 0.0f;
	mParams.addParam( "Terrain Offset Adjust", &mTerrainOffsetAdjust, "max=1.0 min=-1.0 step=0.00001" );*/
}

void Game::draw()
{
	gl::clear( ColorA( 0, 0, 0, 0 ) );
	
	mCamera->update( 0.0f );
	drawBlocks( mBlocks, mCamera->cinderCamera() );
	
	gl::setMatricesWindow( app::getWindowSize(), true );
	// Now draw GUI
	
	if ( mLoadSequence != NULL ) {
		mLoadSequence->draw();
	}
}

void Game::drawBlocks( std::vector<Block*>& blocks, ci::Camera& camera )
{
	gl::enableDepthRead();
	gl::enableAlphaBlending();
	
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	
	mBlockShader.bind();
	mBlockShader.uniform( "modelviewMatrix",		camera.getModelViewMatrix() );
	mBlockShader.uniform( "projectionMatrix",		camera.getProjectionMatrix() );
	mBlockShader.uniform( "eyePos",					camera.getEyePoint() );
	mBlockShader.uniform( "lightPos",				mLight.position );
	mBlockShader.uniform( "lightColor",				mLight.color );
	mBlockShader.uniform( "ambientColor",			ColorA( 0.3f, 0.3f, 0.3f, 1.0f ) );
	mBlockShader.uniform( "numTextures",			(float) kTextureTileSize );
	
	std::vector<Block*>::const_iterator iter;
	for( iter = blocks.begin(); iter != blocks.end(); iter++ ) {
		(*iter)->draw( &mBlockShader );
	}
	mBlockShader.unbind();
	
	glPushMatrix();
	gl::setMatrices( camera );
	gl::color( 1, 1, 0, 1 );
	for( iter = blocks.begin(); iter != blocks.end(); iter++ ) {
		if ( (*iter)->meshType() == BlockMeshNone ) {
			gl::drawCube( (*iter)->mNode->globalPosition(), Vec3f::one() * 0.2f );
		}
	}
	glPopMatrix();

	
	glDisable( GL_CULL_FACE );
}