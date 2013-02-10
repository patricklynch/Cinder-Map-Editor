#pragma once

#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Surface.h"

#include "Node.h"

namespace ly {

typedef enum {
	BlockMeshNone = -1,
	BlockMeshCenter,
	BlockMeshFill,
	BlockMeshEdge,
	BlockMeshWall,
	BlockMeshWallEnd,
	BlockMeshWallDiagonal,
	BlockMeshInnerCorner,
	BlockMeshDoubleInnerCorner,
	BlockMeshOuterCorner
} BlockMeshType;

class Block {
public:
	Block( ci::Vec3i tilePosition = ci::Vec3i::zero() );
	
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
	
	ci::gl::Texture*		mTextureD;
	ci::gl::Texture*		mTextureR;
	ci::gl::Texture*		mTextureG;
	ci::gl::Texture*		mTextureB;
	ci::gl::Texture*		mTextureMask;
	ci::gl::Texture*		mBlankMask;
	ci::gl::Texture*		mTexturePaintMask;
	ci::gl::Texture*		mTextureEdge;
	
	ci::Matrix44f			mTextureMatrix;
	ci::Matrix44f			mTextureMatrix2;
	
private:
	float					testRotation;
	
	ci::gl::VboMesh*		mVboMeshTile;
	BlockMeshType			mBlockMeshType;
	ci::Vec2f				testOffset;
};
	
}