#include "cinder/app/AppBasic.h"

#include "Editor.h"
#include "Game.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace ly {
	
static const float	kFrameRate		= 60.0f;
static const int	kWindowWidth	= 1920;
static const int	kWindowHeight	= 1080;

class MapEditorApp : public AppBasic {
  public:
	void prepareSettings( Settings *settings );
	void setup();
	
	void mouseWheel( ci::app::MouseEvent event ) { ly::Input::get()->mouseWheel( event ); }
	void mouseDown( ci::app::MouseEvent event ) { ly::Input::get()->mouseDown( event ); }
	void mouseMove( ci::app::MouseEvent event ) { ly::Input::get()->mouseMove( event ); }
	void mouseDrag( ci::app::MouseEvent event ) { ly::Input::get()->mouseDrag( event ); }
	void mouseUp  ( ci::app::MouseEvent event ) { ly::Input::get()->mouseUp( event ); }
	void keyDown  ( ci::app::KeyEvent event )   {ly:: Input::get()->keyDown  ( event ); }
	void keyUp    ( ci::app::KeyEvent event )   {ly:: Input::get()->keyUp  ( event ); }
	
	void resize( ResizeEvent event );
	void update();
	void draw();
	
	float					mPrevElapsedSeconds;
	ly::Game*				mGame;
	ly::Editor*				mEditor;
};
	
}

using namespace ly;

void MapEditorApp::prepareSettings( Settings *settings )
{
	settings->setFrameRate( kFrameRate );
	settings->setWindowSize( kWindowWidth, kWindowHeight );
}

void MapEditorApp::setup()
{
	mGame = new Game();
	mEditor = new Editor( mGame );
}

void MapEditorApp::resize( ResizeEvent event ) {}

void MapEditorApp::update()
{
    float currentTime = (float) app::getElapsedSeconds();
    float deltaTime = currentTime - mPrevElapsedSeconds;
    mPrevElapsedSeconds = currentTime;
	
	float timeScale = 1.0f;
    mGame->update( deltaTime * timeScale );
	mEditor->update( deltaTime );
}

void MapEditorApp::draw()
{
	mGame->draw();
	mEditor->draw();
}


CINDER_APP_BASIC( MapEditorApp, RendererGl(0) )
