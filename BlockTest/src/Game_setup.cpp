#include "Game.h"
#include "AssetManager.h"
#include "cinder/Rand.h"

#include "cinder/Surface.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

Game::Game()
{
	setupScene();
	setupRenderer();
}

Game::~Game() {}

void Game::setupScene()
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
		"models/dome.obj",
		"models/geosphere_center.obj",
		"models/geosphere.obj",
		"models/tri_prism.obj",
		"models/plane.obj",
		"textures/texture_tiles.png"
	};
	assetManager->loadAssets( &assets[0], sizeof( assets ) / sizeof( std::string ) );
	
	mCamera = ly::Camera::get();
	mCamera->setFov( 60 );
	mCamera->setZoom( 3 );
	mCamera->rotation.y = 45.0f;
	mCamera->setAngle( -45.0f);
	
	mLight.position = Vec3f( 20, 100, 100 );
	mLight.color = ColorA::white();
	
	mTerrain = new Terrain();
	mTerrain->mNode->setTexture( assetManager->getTexture( "textures/texture_tiles.png" ) );
	mTerrain->mNode->mVboMesh = assetManager->getVboMesh( "models/plane.obj" );
	mTerrain->mNode->colors[ MaterialSpecular ] = ColorA::white() * 0.1f;
	
	{	// Create Blocks
		gl::Texture* texture = assetManager->getTexture( "textures/texture_tiles.png" );
		int n = 8;
		int i = 0;
		for(int x = -n; x < n; x++) {
			for(int z = -n; z < n; z++) {
				for(int y = 0; y < 5; y++) {
					if ( randBool() || randBool()  || randBool() ) continue;
					Block* block = new Block();
					block->mTextureOffset = Vec2i( randInt(0,7), randInt(0,7) );
					block->mNode->scale = Vec3f::one() * kTileSize;
					block->mNode->setTexture( texture );
					block->mNode->mVboMesh = assetManager->getVboMesh( "models/cube_smooth_multiface.obj" );
					block->mNode->position = Vec3f( x, y, z ) * kTileSize;
					mBlocks.push_back( block );
				}
			}
		}
	}
}

void Game::update( const float deltaTime )
{
	mCamera->update( deltaTime);
	mEditorCamera.update( deltaTime );
	ly::Input::get()->update( deltaTime );
	
	for( std::vector<Block*>::const_iterator iter = mBlocks.begin(); iter != mBlocks.end(); iter++ ) {
		Block* block = *iter;
		block->update( deltaTime );
	}
	mTerrain->update( deltaTime );
}
