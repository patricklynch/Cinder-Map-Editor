#pragma once

#include "cinder/Camera.h"
#include "cinder/Vector.h"

#include "Camera.h"
#include "Node.h"
#include "Character.h"
#include "Tile.h"

#include <vector>

namespace ly {

class Game {
public:
	static Game* create( int windowWidth, int windowHeight, int maxTileRadius = 0 );
	static Game* get();
	
	virtual ~Game();
	
	void draw();
	void update( const float deltaTime );
	
	void onJoypadInput( int joypadIndex, float x, float y );
	void onButtonDown( int buttonIndex );
	void onButtonUp( int buttonIndex );
	
	void createLevel();
	
	ci::Vec2f windowSize() { return mWindowSize; }
	ci::Vec2f windowCenter() { return mWindowSize * 0.5f; }
	
	Camera& camera() { return *mCamera; }
	
	void setEditorModeActive( bool value ) { mEditorMode = value; }
	std::vector<Tile*>& tiles() { return mTiles; }
	
private:
	int mMaxVisibleTileRadius;
	
	static Game* sInstance;
	Game( int windowWidth, int windowHeight, int maxTileRadius = 0 );
	
	bool mEditorMode;
	bool mDebugDraw;
	
	ci::Vec2f mWindowSize;
	
	Character* mCharacter;
	ly::Camera* mCamera;
	ci::CameraPersp mCam;
	
	ci::Vec2i mPrevCenter;
	std::vector<Node*> mNodes;
	std::vector<Tile*> mTiles;
	bool mHastUpdated;
};

}