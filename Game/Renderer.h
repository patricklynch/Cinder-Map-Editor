#pragma once

#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"

#include <map>

#include "Node.h"
#include "Light.h"
#include "Camera.h"

namespace ly {
	
typedef enum {
	SpriteUnlit,
	Debug,
	Sprite,
	DepthMap,
	DepthMap2,
	Mega
} ShaderType;
	
const static int kMaxLights				= 4;
static const int kShadowMapResolution	= 1024;
	
struct DebugDrawSettings {
	bool lightShadowCamFrustrums;
	bool shadowMapPreviews;
	bool origin;
};
	
class Renderer {
public:
	static Renderer* get();
	virtual ~Renderer() {}
	
	void setup();
	void draw();
	void update( const float deltaTime );	

	void setCamera( ly::Camera* camera ) { mMainCamera = camera; }
	void setShadowCastingLight( ly::Light* light );
	ly::Camera* currentCamera() const { return mMainCamera; }
	void addLight( ly::Light* light );
	void removeLight( ly::Light* light );
	void addNode( Node* node );
	void addNodes( std::vector<Node*>& node );
	void removeNode( Node* node );
	
	static DebugDrawSettings debugDraw;
	static bool debugDrawEnabled;
	
	ci::gl::Fbo* createDepthMap( int width, int height );
	ci::gl::Fbo* createShadowDepthMap();
	ci::gl::Fbo* createDepthMap();
	
private:
	void drawSprites( std::vector<Node*>& nodes );
	void drawObjects( std::vector<Node*>& nodes );
	void drawObjects( std::vector<Node*>& nodes, ci::gl::GlslProg* shader );
	void drawDebug();
	
	static Renderer* sInstance;
	Renderer();
	
	ly::Camera* mMainCamera;
	ly::Light* mShadowCastingLight;
	
	// Shaders
	std::map<ShaderType, ci::gl::GlslProg> mShaders;
	
	// Default textures
	ci::gl::Texture mDefaultBlack;
	ci::gl::Texture mDefaultWhite;
	ci::gl::Texture mDefaultGray;
	
	// Fbos
	ci::gl::Fbo* mSceneDepthMap;
	ci::gl::Fbo* mShadowDepthMap;
	
	std::vector<ly::Light*> mLights;
	std::vector<Node*> mNodes;
	
	ci::ColorA mFogColor;
};

}