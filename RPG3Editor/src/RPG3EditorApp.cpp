#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/ImageIo.h"

#include "cinder/params/Params.h"

#include "Resources.h"
#include "Constants.h"

#include "Input.h"
#include "EditorMode.h"
#include "Game.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace ly {

class RPG3EditorApp : public AppBasic {
public:
	void prepareSettings( Settings *settings );
	void setup();
	void resize( ResizeEvent event );
	void update();
	void draw();
	
	void mouseDown( ci::app::MouseEvent event ) { Input::get()->mouseDown( event ); }
	void mouseMove( ci::app::MouseEvent event ) { Input::get()->mouseMove( event ); }
	void mouseDrag( ci::app::MouseEvent event ) { Input::get()->mouseDrag( event ); }
	void mouseUp  ( ci::app::MouseEvent event ) { Input::get()->mouseUp( event ); }
	void keyDown  ( ci::app::KeyEvent event )   { Input::get()->keyDown  ( event ); }
	
private:
	params::InterfaceGl mParams;
	Game* mGame;
	EditorMode* mEditorMode;
	float mPrevElapsedSeconds;
};

void RPG3EditorApp::prepareSettings( Settings *settings )
{
	settings->setFrameRate( kFrameRate );
	settings->setWindowSize( kWindowWidth, kWindowHeight );
}

void RPG3EditorApp::setup()
{
	mParams = params::InterfaceGl( "Parameters", Vec2i( kParamsWidth, kParamsHeight ) );
	mGame = Game::create( app::getWindowWidth(), app::getWindowHeight(), 30 );
	mEditorMode = new EditorMode( mGame );
}

void RPG3EditorApp::resize( ResizeEvent event ) {}

void RPG3EditorApp::update()
{
	float currentTime = (float)app::getElapsedSeconds();
	float deltaTime = currentTime - mPrevElapsedSeconds;
	mPrevElapsedSeconds = currentTime;
	
	Input::get()->update( deltaTime );
	mGame->update( deltaTime );
	mEditorMode->update( deltaTime );
}

void RPG3EditorApp::draw()
{
	gl::clear( kClearColor );
	mGame->draw();
	mEditorMode->draw();
}

}

CINDER_APP_BASIC( ly::RPG3EditorApp, RendererGl(0) )
