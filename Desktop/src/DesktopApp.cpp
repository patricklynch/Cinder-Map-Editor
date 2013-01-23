#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/ImageIo.h"

#include <boost/bind.hpp>

#include "Resources.h"
#include "Constants.h"
#include "Input.h"
#include "Game.h"
#include "EditorMode.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace ly {

class DesktopApp : public AppBasic {
public:
	void mouseDown( ci::app::MouseEvent event ) { Input::get()->mouseDown( event ); }
	void mouseMove( ci::app::MouseEvent event ) { Input::get()->mouseMove( event ); }
	void mouseDrag( ci::app::MouseEvent event ) { Input::get()->mouseDrag( event ); }
	void mouseUp  ( ci::app::MouseEvent event ) { Input::get()->mouseUp( event ); }
	void keyDown  ( ci::app::KeyEvent event )   { Input::get()->keyDown  ( event ); }
	
	void prepareSettings( Settings *settings );
	void setup();
	void resize( ResizeEvent event );
	void update();
	void draw();
	
	void toggleEditorMode();
	
private:
	bool mEditorMode;
	float mPrevElapsedSeconds;
	EditorMode* mEditor;
	Game* mGame;
};
	
}

using namespace ly;

void DesktopApp::prepareSettings( Settings *settings )
{
	settings->setFrameRate( kFrameRate );
	settings->setWindowSize( kWindowWidth, kWindowHeight );
}

void DesktopApp::setup()
{	
	mGame = Game::create( kWindowWidth, kWindowHeight, 20 );
	mEditor = new EditorMode( mGame );
	
	mEditorMode = true;
	ly::Input::get()->addListenerForKey( boost::bind( &DesktopApp::toggleEditorMode, this ), KeyEvent::KEY_e );
}

void DesktopApp::toggleEditorMode()
{
	mEditorMode = !mEditorMode;
}

void DesktopApp::resize( ResizeEvent event ) {}

void DesktopApp::update()
{
    float currentTime = app::getElapsedSeconds();
    float deltaTime = currentTime - mPrevElapsedSeconds;
    mPrevElapsedSeconds = currentTime;
	
	mGame->update( deltaTime );
	if ( mEditorMode ) mEditor->update( deltaTime );
}

void DesktopApp::draw()
{
	gl::clear( kClearColor );
	
	mGame->draw();
	if ( mEditorMode ) mEditor->draw();
}

CINDER_APP_BASIC( DesktopApp, RendererGl(0) )
