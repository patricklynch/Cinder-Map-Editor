#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/params/Params.h"
#include "cinder/gl/Vbo.h"
#include "cinder/Surface.h"

#include "Camera.h"
#include "Input.h"
#include "GlslInclude.h"
#include "EditorCamera.h"
#include "Block.h"
#include "Terrain.h"
#include "AssetManager.h"
#include "Resources.h"
#include "GameConstants.h"

namespace ly {
		
struct SimpleLight {
	ci::Vec3f position;
	ci::ColorA color;
};
	
class IGameEventDelegate {
public:
	virtual void mapCenterDidUpdate( ci::Vec3i center ) = 0;
};

class Game {
public:
							Game();
	virtual					~Game();
	void					update( const float deltaTime );
	void					draw();
	
	std::vector<Block*>&	blocks() { return mBlocks; }
	
	static ci::Vec3f		blockResetPosition( ci::Vec3f tilePosition, ci::Vec3f center, int radius );
	IGameEventDelegate*		mDelegate;
	
private:
	SimpleLight				mLight;
	ly::Camera*				mCamera;
	float					mPrevElapsedSeconds;
	ly::EditorCamera		mEditorCamera;
	ci::gl::GlslProg		mBlockShader;
	ci::gl::GlslProg		mTerrainShader;
	std::vector<Block*>		mBlocks;
	Terrain*				mTerrain;
	ci::Vec3i				mPrevMapCenter;
	int						mMaxVisibleTileRadius;
	
	void					setupScene();
	void					setupRenderer();
	void					updateLocation();
	
	ci::params::InterfaceGl mParams;
	
};

}