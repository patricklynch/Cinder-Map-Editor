#include "cinder/app/AppBasic.h"

#include "Game.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace ly {
	
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
	
	float					mPrevElapsedSeconds;
	ly::Game*				mGame;
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
	mGame = new Game();
}

void BlockTestApp::resize( ResizeEvent event ) {}

void BlockTestApp::update()
{
    float currentTime = (float) app::getElapsedSeconds();
    float deltaTime = currentTime - mPrevElapsedSeconds;
    mPrevElapsedSeconds = currentTime;
	
    mGame->update( deltaTime );
}

void BlockTestApp::draw()
{
	mGame->draw();
}


CINDER_APP_BASIC( BlockTestApp, RendererGl(0) )
