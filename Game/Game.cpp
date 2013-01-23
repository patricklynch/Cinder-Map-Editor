#include "Game.h"
#include "Renderer.h"
#include "Animator.h"
#include "Tile.h"
#include "GameConstants.h"
#include "AssetManager.h"

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/Rand.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

Game* Game::sInstance = NULL;

Game* Game::create( int windowWidth, int windowHeight, int maxTileRadius )
{
	if ( !sInstance ) sInstance = new Game( windowWidth, windowHeight, maxTileRadius );
	else return NULL;
	return sInstance;
}

Game* Game::get()
{
	return sInstance;
}

Game::Game( int windowWidth, int windowHeight, int maxTileRadius ) : mHastUpdated( false ), mDebugDraw( false ), mCharacter( NULL ), mEditorMode( false )
{
	mWindowSize = Vec2f( windowWidth, windowHeight );
	if ( maxTileRadius == 0 ) mMaxVisibleTileRadius = kDefaultMaxVisibleTileRadius;
	else mMaxVisibleTileRadius = maxTileRadius;
	
	mCamera = ly::Camera::get();
	mCamera->position.y = 0.0f;
	mCamera->setFov( 55 );
	mCamera->setZoom( 5 );
	mCamera->setAngle( -90 );
	
	gl::Texture::Format format;
	format.enableMipmapping();
	format.setMagFilter( GL_NEAREST );
	
	//mCharacter = new Character();
	
	preloadAssets();
	
	ly::Light* light = new ly::Light();
	light->node().position = Vec3f( 0, 10, 0 );
	ly::Renderer::get()->setShadowCastingLight( light );
	
	ly::Renderer::get()->setCamera( mCamera );
	
	Node* node = new Node();
	node->setMesh( AssetManager::get()->getModel( "models/yoda.obj" ) );
	node->setTexture( AssetManager::get()->getTexture( "textures/texture_tiles.png" ) );
	ly::Renderer::get()->addNode( node );
	
	Vec2i center = mCharacter ? Tile::tilePosition( mCharacter->node().position ) : Vec2i::zero();
	
	float n = 4;
	for(int x = -n; x <= n; x++) {
		for(int y = -n; y <= n; y++) {
			Node* node = new Node();
			node->position = Vec3f( x, 0.0f, y ) * 1.0f;
			//node->scale = Vec3f::one() * (float) kTileSize;
			//node->setTexture( AssetManager::get()->getTexture( "textures/texture_tiles.png" ) );
			node->setColor( ColorA( 1, 0, 0, 0 ) );
			node->setMesh( AssetManager::get()->getModel( "models/cube.obj" ) );
			ly::Renderer::get()->addNode( node );
		}
	}
	
}

Game::~Game() {}

void Game::preloadAssets()
{
	std::vector<std::string> testAssets;
	std::string testAssetsArr[] = {
		"shaders/passthru_mvp.vert",
		"shaders/unlit.frag",
		
		"fonts/DisposableDroidBB_bld.ttf",
		"fonts/DisposableDroidBB.ttf",
		
		"textures/default_black.png",
		"textures/default_white.png",
		"textures/default_gray.png",
		"textures/texture_tiles.png",
		
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
	};
	for(int i = 0; i < sizeof( testAssetsArr ) / sizeof( std::string); i++ )
		testAssets.push_back( testAssetsArr[i] );
	AssetManager::get()->loadAssets( testAssets );
}

void Game::onButtonDown( int buttonIndex ) {}

void Game::onButtonUp( int buttonIndex ) {}

void Game::onJoypadInput( int joypadIndex, float x, float y )
{
	//mCharacter->move( Vec2f( x, -y ) );
}

void Game::update( const float deltaTime )
{
	Animator::update( deltaTime );
	ly::Renderer::get()->update( deltaTime );
	mCamera->update( deltaTime );
	
	//mCharacter->update( deltaTime );
	
	//if ( !mEditorMode )
	//	mCamera->position += (mCharacter->focalPoint() - mCamera->position ) / 10.0f;
	
	
	/*Vec2i center = Tile::tilePosition( mCamera->position );
	if ( center != mPrevCenter ) {
		for( std::vector<Tile*>::iterator iter = mTiles.begin(); iter != mTiles.end(); iter++ ) {
			Vec2i pos = Vec2i( (*iter)->x, (*iter)->y );
			Vec2i diff = pos - center;
			int n = mMaxVisibleTileRadius;
			if ( diff.x > n )			(*iter)->x -= n*2+1;
			else if ( diff.x < -n )		(*iter)->x += n*2+1;
			if ( diff.y > n )			(*iter)->y -= n*2+1;
			else if ( diff.y < -n )		(*iter)->y += n*2+1;
		}
		for( std::vector<Tile*>::iterator iter = mTiles.begin(); iter != mTiles.end(); iter++ ) {
			if ( (*iter)->x == center.x && (*iter)->y == center.y ) {
				mCharacter->onEnterTile( **iter );
				break;
			}
		}
	}
	mPrevCenter = center;
	
	
	for( std::vector<Block*>::iterator iter = mBlocks.begin(); iter != mBlocks.end(); iter++ )
		(*iter)->update( deltaTime );
	
	for( std::vector<Tile*>::iterator iter = mTiles.begin(); iter != mTiles.end(); iter++ )
		(*iter)->update( deltaTime );*/
	
	mHastUpdated = true;
}

void Game::draw()
{
	if ( !mHastUpdated ) return;
	
	ly::Renderer::get()->draw();
}

