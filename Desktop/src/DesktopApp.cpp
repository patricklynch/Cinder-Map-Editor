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

class DesktopApp : public AppBasic {
  public:
	void prepareSettings( Settings *settings );
	void setup();
	void resize( ResizeEvent event );
	void update();
	void draw();
	
	params::InterfaceGl		mParams;
};

void DesktopApp::prepareSettings( Settings *settings )
{
	settings->setFrameRate( kFrameRate );
	settings->setWindowSize( kWindowWidth, kWindowHeight );
}

void DesktopApp::setup()
{
	mParams = params::InterfaceGl( "Parameters", Vec2i( kParamsWidth, kParamsHeight ) );
	/*mParams.addParam( "Mix Red", &mMixColorRed, "min=-1.0 max=1.0 step=0.01 keyIncr=r keyDecr=R" );
	mParams.addParam( "Mix Green", &mMixColorGreen, "min=-1.0 max=1.0 step=0.01 keyIncr=g keyDecr=G" );
	mParams.addParam( "Mix Blue", &mMixColorBlue, "min=-1.0 max=1.0 step=0.01 keyIncr=b keyDecr=B" );*/
}

void DesktopApp::resize( ResizeEvent event )
{
}


void DesktopApp::update()
{
}

void DesktopApp::draw()
{
	gl::clear( kClearColor );
	
	params::InterfaceGl::draw();
}

CINDER_APP_BASIC( DesktopApp, RendererGl(0) )
