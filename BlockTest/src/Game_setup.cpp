#include "Game.h"
#include "AssetManager.h"
#include "cinder/Rand.h"

#include "cinder/Surface.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

Game::Game() : mDelegate( NULL )
{
	setupScene();
	setupRenderer();
}

Game::~Game() {}

void Game::setupScene()
{
	AssetManager* assetManager = AssetManager::get();
	
	mCamera = ly::Camera::get();
	mCamera->setFov( 45 );
	mCamera->setZoom( 30 );
	mCamera->rotation.y = 45.0f;
	mCamera->setAngle( -45.0f);
	
	mLight.position = Vec3f( 20, 100, 100 );
	mLight.color = ColorA::white();
	
	mTerrain = new Terrain();
	mTerrain->mNode->setTexture( assetManager->getTexture( "textures/texture_tiles.png" ) );
	mTerrain->mNode->mVboMesh = assetManager->getVboMesh( "models/plane.obj" );
	mTerrain->mNode->colors[ MaterialSpecular ] = ColorA::white() * 0.1f;
	
	mMaxVisibleTileRadius = kDefaultMaxVisibleTileRadius;
	
	// Terrain tile blocks
	int n = mMaxVisibleTileRadius;
	for(int x = -n; x < n; x++) {
		for(int z = -n; z < n; z++) {
			int y = 0;
			//if ( randFloat() > 0.1f && ( x > -6 && x < 6 )) y = 1;
			Vec3i tilePos = Vec3f( x, y, z );
			mBlocks.push_back( Block::createBlock( tilePos ) );
		}
	}
}

ci::Vec3f Game::blockResetPosition( ci::Vec3f tilePosition, ci::Vec3f center, int radius )
{
	int n = radius;
	int edgeResetDist = n*2;
	Vec3f diff = tilePosition - center;
	Vec3f output = tilePosition;
	if ( diff.x > n )			output.x -= edgeResetDist;
	else if ( diff.x < -n )		output.x += edgeResetDist;
	if ( diff.z > n )			output.z -= edgeResetDist;
	else if ( diff.z < -n )		output.z += edgeResetDist;
	return output;
}

void Game::updateLocation()
{
	Vec3f realCenter = mCamera->position;
	Vec3i mapCenter = Terrain::mapLocation( realCenter );
	bool mapCenterDidChange = mapCenter != mPrevMapCenter;
	mPrevMapCenter = mapCenter;
	
	if ( mapCenterDidChange ) {
		if ( mDelegate != NULL ) {
			mDelegate->mapCenterDidUpdate( mapCenter );
		}
		for( std::vector<Block*>::iterator iter = mBlocks.begin(); iter != mBlocks.end(); iter++ ) {
			Block* block = *iter;
			Vec3i mapPos = Terrain::mapLocation( block->mNode->position );
			Vec3f realPos = Terrain::realPosition( blockResetPosition( mapPos, mapCenter, mMaxVisibleTileRadius ) );
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
	mTerrain->update( deltaTime );
}








