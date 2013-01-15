#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/ImageIo.h"

#include "cinder/params/Params.h"

#include "Resources.h"
#include "Constants.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class GameApp : public AppBasic {
  public:
	void prepareSettings( Settings *settings );
	void setup();
	void mouseDown( MouseEvent event );
	void keyDown( KeyEvent event );
	void resize( ResizeEvent event );
	void update();
	void draw();

	gl::Texture				mTexture;
	gl::GlslProg			mShader;
	gl::Fbo					mFbo;
	params::InterfaceGl		mParams;

	float mMixColorRed;
	float mMixColorGreen;
	float mMixColorBlue;

};

void GameApp::prepareSettings( Settings *settings )
{
	settings->setFrameRate( kFrameRate );
	settings->setWindowSize( kWindowWidth, kWindowHeight );
}

void GameApp::setup()
{
	try {
		mShader = gl::GlslProg( loadResource( RES_SHADER_PASSTHRU ), loadResource( RES_SHADER_FRAGMENT ) );
	} catch ( gl::GlslProgCompileExc &exc ) {
		console() << "Cannot compile shader: " << exc.what() << std::endl;
		exit(1);
	}catch ( Exception &exc ){
		console() << "Cannot load shader: " << exc.what() << std::endl;
		exit(1);
	}

	mFbo = gl::Fbo( kWindowWidth, kWindowHeight );
	try {
		mTexture = gl::Texture( loadImage( loadResource( RES_GRADIENT ) ) );
	}catch ( Exception &exc ){
		console() << "Cannot load texture: " << exc.what() << std::endl;
	}

	mMixColorRed = 0.0f;
	mMixColorGreen = 0.0f;
	mMixColorBlue = 0.0f;
	
	mParams = params::InterfaceGl( "Parameters", Vec2i( kParamsWidth, kParamsHeight ) );
	mParams.addParam( "Mix Red", &mMixColorRed, "min=-1.0 max=1.0 step=0.01 keyIncr=r keyDecr=R" );
	mParams.addParam( "Mix Green", &mMixColorGreen, "min=-1.0 max=1.0 step=0.01 keyIncr=g keyDecr=G" );
	mParams.addParam( "Mix Blue", &mMixColorBlue, "min=-1.0 max=1.0 step=0.01 keyIncr=b keyDecr=B" );
}

void GameApp::mouseDown( MouseEvent event )
{
}

void GameApp::keyDown( KeyEvent event )
{
	if ( event.getCode() == KeyEvent::KEY_f ){
		setFullScreen( !isFullScreen() );
	}
}

void GameApp::resize( ResizeEvent event )
{
	mFbo = gl::Fbo( getWindowWidth(), getWindowHeight() );
}


void GameApp::update()
{
	// Do something with your texture here.
	
}

void GameApp::draw()
{
	// clear out the window with black
	gl::clear( kClearColor );
	
	if( !mTexture ) return;
	mFbo.bindFramebuffer();
	mTexture.enableAndBind();
	mShader.bind();
	mShader.uniform( "tex", 0 );
	mShader.uniform( "mixColor", Vec3d( mMixColorRed, mMixColorGreen, mMixColorBlue ) );
	gl::drawSolidRect( getWindowBounds() );
	mTexture.unbind();
	mShader.unbind();
	mFbo.unbindFramebuffer();
	
	gl::Texture fboTexture = mFbo.getTexture();
	fboTexture.setFlipped();
	gl::draw( fboTexture );

	params::InterfaceGl::draw();
}


CINDER_APP_BASIC( GameApp, RendererGl(0) )
