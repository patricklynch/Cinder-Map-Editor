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
#include "AssetManager.h"
#include "Resources.h"
#include "Constants.h"
#include "GameConstants.h"

namespace ly {
		
struct SimpleLight {
	ci::Vec3f position;
	ci::ColorA color;
};

class Game {
public:
							Game();
	virtual					~Game();
	
	void					setupScene();
	void					setupRenderer();
	void					update( const float deltaTime );
	void					draw();
	
	SimpleLight				mLight;
	ly::Camera*				mCamera;
	float					mPrevElapsedSeconds;
	ly::EditorCamera		mEditorCamera;
	ci::gl::GlslProg		mBlockShader;
	ci::gl::GlslProg		mTerrainShader;
	std::vector<Block*>		mBlocks;
	Terrain*				mTerrain;
	
private:
	
};

}