#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/params/Params.h"
#include "cinder/gl/Vbo.h"
#include "cinder/Vector.h"
#include "cinder/Surface.h"

#include "Camera.h"
#include "Input.h"
#include "GlslInclude.h"
#include "EditorCamera.h"
#include "Block.h"
#include "Terrain.h"
#include "AssetManager.h"
#include "GameConstants.h"
#include "LoadSequence.h"

namespace ly {
		
struct SimpleLight {
	ci::Vec3f position;
	ci::ColorA color;
};
	
class IGameEventDelegate {
public:
	virtual void mapCenterDidUpdate( ci::Vec3i center ) = 0;
};

class Game : public IAssetManagerDelegate {
public:
	Game();
	virtual ~Game();

	void							update( const float deltaTime );
	void							draw();
	void							drawBlocks( std::vector<Block*>& blocks, ci::Camera& camera );
	virtual void					assetPreloaded( int current, int total );
	
	std::vector<Block*>&			blocks() { return mBlocks; }
	
	static ci::Vec3f				blockResetPosition( ci::Vec3f tilePosition, ci::Vec3f center, int radius );
	IGameEventDelegate*				mDelegate;
	
	Block*							addBlock( ci::Vec3i atTilePosition );
	void							removeBlock( Block* block );
	
	static ci::Vec3i				mapLocation( ci::Vec3f realPosition );
	static ci::Vec3f				realPosition( ci::Vec3i tilePosition );
	
	std::vector<Terrain*>&			blockTerrains() { return mBlockTerrains; }
	std::vector<ci::gl::Texture*>&	blockTextures() { return mBlockTextures; };
	
private:
	std::vector<Terrain*>			mBlockTerrains;
	std::vector<ci::gl::Texture*>	mBlockTextures;

	SimpleLight						mLight;
	ly::Camera*						mCamera;
	float							mPrevElapsedSeconds;
	ly::EditorCamera				mEditorCamera;
	ci::gl::GlslProg				mBlockShader;
	std::vector<Block*>				mBlocks;
	ci::Vec3i						mPrevMapCenter;
	int								mMaxVisibleTileRadius;
	
	void							setupScene();
	void							setupRenderer();
	void							updateLocation();
	
	ci::params::InterfaceGl			mParams;
	LoadSequence*					mLoadSequence;
};

}