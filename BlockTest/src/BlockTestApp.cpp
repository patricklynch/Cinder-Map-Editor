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
	
class BlockTestApp : public AppBasic {
  public:
	void prepareSettings( Settings *settings );
	void setup();
	
	void mouseWheel( ci::app::MouseEvent event ) { ly::Input::get()->mouseWheel( event ); }
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
	ly::Editor*				mEditor;
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
		"models/edge_straight.obj",
		"models/edge_diagonal.obj",
		"models/edge_inner_corner.obj",
		"models/dome.obj",
		"models/geosphere_center.obj",
		"models/geosphere.obj",
		"models/tri_prism.obj",
		"models/plane.obj",
		"textures/texture_tiles.png"
	};
	assetManager->loadAssets( &assets[0], sizeof( assets ) / sizeof( std::string ) );
	
	mGame = new Game();
	mEditor = new Editor( mGame );
}

void BlockTestApp::resize( ResizeEvent event ) {}

void BlockTestApp::update()
{
    float currentTime = (float) app::getElapsedSeconds();
    float deltaTime = currentTime - mPrevElapsedSeconds;
    mPrevElapsedSeconds = currentTime;
	
	float timeScale = 1.0f;
    mGame->update( deltaTime * timeScale );
	mEditor->update( deltaTime );
}

void BlockTestApp::draw()
{
	mGame->draw();
	mEditor->draw();
}


CINDER_APP_BASIC( BlockTestApp, RendererGl(0) )
