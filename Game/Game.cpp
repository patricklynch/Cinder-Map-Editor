#include "Game.h"
#include "Renderer.h"
#include "Animator.h"
#include "Tile.h"

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
	
	createLevel();
	mCharacter = new Character();
}

Game::~Game() {}

void Game::createLevel()
{
	Vec2i center = mCharacter ? Tile::tilePosition( mCharacter->node().position ) : Vec2i::zero();
	
	float n = mMaxVisibleTileRadius;
	for(int x = -n; x <= n; x++) {
		for(int y = -n; y <= n; y++) {
			Tile* tile = new Tile( x, y );
			mTiles.push_back( tile );
		}
	}
}

void Game::onButtonDown( int buttonIndex )
{
}

void Game::onButtonUp( int buttonIndex )
{
}

void Game::onJoypadInput( int joypadIndex, float x, float y )
{
	mCharacter->move( Vec2f( x, -y ) );
}

void Game::update( const float deltaTime )
{
	Animator::update( deltaTime );
	
	mCharacter->update( deltaTime );
	
	if ( !mEditorMode )
		mCamera->position += (mCharacter->focalPoint() - mCamera->position ) / 10.0f;
	mCamera->update( deltaTime );
	
	Vec2i center = Tile::tilePosition( mCamera->position );
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
	
	for( std::vector<Tile*>::iterator iter = mTiles.begin(); iter != mTiles.end(); iter++ ) {
		(*iter)->update( deltaTime );
	}
	
	mHastUpdated = true;
}

void Game::draw()
{
	if ( !mHastUpdated ) return;
	
	ly::Renderer::get()->draw();
}

