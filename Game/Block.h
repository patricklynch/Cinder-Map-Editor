#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Surface.h"

#include "Node.h"
#include "Terrain.h"

namespace ly {

class Game;

class Block {
public:
	Block( Game* game );
	
	virtual					~Block();
	
	void					update( const float deltaTime );
	void					draw( ci::gl::GlslProg* shader );
	ci::Vec2i				mTextureOffset;
	Node*					mNode;
	ci::Vec3i				tilePosition;
	float					mRotation;
	
	bool					operator==( Block* other ) { return other->tilePosition == tilePosition; }
	
	void					setMeshType( BlockMeshType type, float rotation );
	BlockMeshType			meshType() const { return mBlockMeshType; }
	
	ci::gl::Texture*		mBlankMask;
	ci::gl::Texture*		mTexturePaintMask;
	
	ci::Matrix44f			mTextureMatrix;
	ci::Matrix44f			mTextureMatrix2;

	void					setTilePosition( ci::Vec3i tilePosition );
	void					setTerrainIndex( int index );
	int						terrainIndex() const { return mTerrainIndex; }
	
private:
	Game*					mGame;
	int						mTerrainIndex;
	Terrain*				mTerrain;
	float					testRotation;
	
	ci::gl::VboMesh*		mVboMeshTile;
	BlockMeshType			mBlockMeshType;
	ci::Vec2f				testOffset;
};
	
}