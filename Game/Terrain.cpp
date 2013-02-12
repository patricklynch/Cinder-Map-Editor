#include "Terrain.h"
#include "AssetManager.h"

#include "cinder/app/App.h"

using namespace ly;

using namespace ci;
using namespace ci::app;

Terrain::Terrain() {}

Terrain::Terrain( std::string name )
{
	load( name );
}

Terrain::~Terrain() {}

void Terrain::load( std::string name )
{
	std::string pathPrefix					= "terrain/" + name + "/";
	AssetManager* assetManager				= AssetManager::get();
	mTexture								= assetManager->getTexture( pathPrefix + "texture.png" );
	mMaskTexture							= assetManager->getTexture( pathPrefix + "mask.png" );
	
	std::string edgePathPrefix				= pathPrefix + "edge/";
	mEdgeMeshes[ BlockMeshCenter ]			= assetManager->getVboMesh( edgePathPrefix + "center.obj" );
	mEdgeMeshes[ BlockMeshWall ]			= assetManager->getVboMesh( edgePathPrefix + "wall.obj" );
	mEdgeMeshes[ BlockMeshCorner ]			= assetManager->getVboMesh( edgePathPrefix + "corner.obj" );
	mEdgeMeshes[ BlockMeshInnerCorner ]		= assetManager->getVboMesh( edgePathPrefix + "inner_corner.obj" );
	mEdgeMeshes[ BlockMeshBridge ]			= assetManager->getVboMesh( edgePathPrefix + "bridge.obj" );
	mEdgeMeshes[ BlockMeshNone ]			= NULL;
	
	std::string tilePathPrefix				= pathPrefix + "tile/";
	mTileMeshes[ BlockMeshCenter ]			= assetManager->getVboMesh( tilePathPrefix + "center.obj" );
	mTileMeshes[ BlockMeshWall ]			= assetManager->getVboMesh( tilePathPrefix + "wall.obj" );
	mTileMeshes[ BlockMeshCorner ]			= assetManager->getVboMesh( tilePathPrefix + "corner.obj" );
	mTileMeshes[ BlockMeshInnerCorner ]		= assetManager->getVboMesh( tilePathPrefix + "inner_corner.obj" );
	mTileMeshes[ BlockMeshBridge ]			= assetManager->getVboMesh( tilePathPrefix + "bridge.obj" );
	mTileMeshes[ BlockMeshNone ]			= NULL;
}