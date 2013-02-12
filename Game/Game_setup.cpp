#include "Game.h"
#include "AssetManager.h"
#include "cinder/Rand.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

Game::Game() : mDelegate( NULL ), mLoadSequence( NULL )
{
	setupScene();
	setupRenderer();
}

Game::~Game() {}

void Game::setupScene()
{
	// Preload assets
	mLoadSequence = new LoadSequence();
	AssetManager* assetManager = AssetManager::get();
	//assetManager->loadDirectory( "models", true );
	assetManager->loadDirectory( "textures", true );
	assetManager->loadDirectory( "terrain", true );
	assetManager->loadDirectory( "brushes", true );
	assetManager->setDelegate( this );
	delete mLoadSequence;
	mLoadSequence = NULL;
	
	mBlockTerrains.push_back( new Terrain( "outcropping" ) );
	mBlockTerrains.push_back( new Terrain( "citywall" ) );

	mBlockTextures.push_back( assetManager->getTexture( "textures/grass.png" ) );
	mBlockTextures.push_back( assetManager->getTexture( "textures/stone.png" ) );
	mBlockTextures.push_back( assetManager->getTexture( "textures/concrete.png" ) );
	mBlockTextures.push_back( assetManager->getTexture( "textures/brick.png" ) );
	
	mCamera = ly::Camera::get();
	mCamera->setFov( 45 );
	mCamera->setZoom( 30 );
	mCamera->rotation.y = 45.0f;
	mCamera->setAngle( -45.0f);
	
	mLight.position = Vec3f( 100, 100, -100 );
	mLight.color = ColorA::white();
	
	mMaxVisibleTileRadius = kDefaultMaxVisibleTileRadius;
	
	// TODO: Load existing blocks
	// For now, create a flat plain of blocks
	int n = mMaxVisibleTileRadius;
	for(int x = -n; x <= n; x++) {
		for(int z = -n; z <= n; z++) {
			addBlock( Vec3f( x, 0, z ) );
		}
	}
}

Block* Game::addBlock( ci::Vec3i atTilePosition )
{
	Block* block = new Block( this );
	block->setTilePosition( atTilePosition );
	mBlocks.push_back( block );
	return block;
}

void Game::assetPreloaded( int current, int total )
{
	float progress = (float) current / (float) total;
	console() << "progress = " << progress << std::endl;
	if ( mLoadSequence != NULL ) {
		mLoadSequence->setProgress( progress );
		if ( progress >= 1.0f ) {
			delete mLoadSequence;
			mLoadSequence = NULL;
		}
	}
}

void Game::removeBlock( Block* block )
{
	std::vector<Block*>::iterator match = std::find( mBlocks.begin(), mBlocks.end(), block );
	if ( match != mBlocks.end() ) {
		delete *match;
		mBlocks.erase( match );
	}
}

ci::Vec3f Game::blockResetPosition( ci::Vec3f tilePosition, ci::Vec3f center, int radius )
{
	int n = radius;
	int edgeResetDist = n*2+1;
	Vec3f diff = tilePosition - center;
	Vec3f output = tilePosition;
	if ( diff.x > n )			output.x -= edgeResetDist;
	else if ( diff.x < -n )		output.x += edgeResetDist;
	if ( diff.z > n )			output.z -= edgeResetDist;
	else if ( diff.z < -n )		output.z += edgeResetDist;
	return output;
}

Vec3i Game::mapLocation( Vec3f realPosition )
{
	float dx = realPosition.x < 0.0f ? -1.0f : 1.0f;
	float dy = realPosition.y < 0.0f ? -1.0f : 1.0f;
	float dz = realPosition.z < 0.0f ? -1.0f : 1.0f;
	return Vec3i(realPosition.x / kTileSize + kTileSize * 0.5f * dx,
				 realPosition.y / kTileSize + kTileSize * 0.5f * dy,
				 realPosition.z / kTileSize + kTileSize * 0.5f * dz);
}

Vec3f Game::realPosition( Vec3i tilePosition )
{
	return tilePosition * kTileSize;
}

void Game::updateLocation()
{
	Vec3f realCenter = mCamera->position;
	Vec3i mapCenter = Game::mapLocation( realCenter );
	bool mapCenterDidChange = mapCenter != mPrevMapCenter;
	mPrevMapCenter = mapCenter;
	
	if ( mapCenterDidChange ) {
		if ( mDelegate != NULL ) {
			mDelegate->mapCenterDidUpdate( mapCenter );
		}
		for( std::vector<Block*>::iterator iter = mBlocks.begin(); iter != mBlocks.end(); iter++ ) {
			Block* block = *iter;
			Vec3i mapPos = Game::mapLocation( block->mNode->position );
			Vec3f realPos = Game::realPosition( blockResetPosition( mapPos, mapCenter, mMaxVisibleTileRadius ) );
			block->mNode->position.x = realPos.x;
			block->mNode->position.z = realPos.z;
		}
	}
}

void Game::update( const float deltaTime )
{
	mCamera->update( deltaTime);
	mEditorCamera.update( deltaTime );
	ly::Input::get()->update( deltaTime );
	
	updateLocation();
	
	for( std::vector<Block*>::const_iterator iter = mBlocks.begin(); iter != mBlocks.end(); iter++ ) {
		Block* block = *iter;
		block->update( deltaTime );
	}
}








