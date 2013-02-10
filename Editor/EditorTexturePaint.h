#pragma once

#include "GameConstants.h"
#include "Camera.h"

#include "cinder/Vector.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Texture.h"

namespace ly {
	
class Editor;
	
struct TexturePaintState {
	int brushIndex;
	int textureIndex;
	int brushSize;
	float brushStrength;
};
	
// TODO: Separate private/public and make getters/setters
	
static const ci::Vec3f kTexturePaintNullPosition	= ci::Vec3f( -999,-999,-999 );
static const int kTexturePaintMapResolution			= 1024;

class EditorTexturePaint {
public:
	EditorTexturePaint( TexturePaintState defaultState );
	virtual ~EditorTexturePaint();
	
	void draw();
	void update( const float deltaTime );
	void debugDraw();
	
	void setTarget( ci::Vec3f position );
	void apply( ci::Vec3f position = kTexturePaintNullPosition );
	void setState( TexturePaintState state ) { mState = state; }
	
	void editingStarted();
	void editingComplete();
	bool isApplying() const { return mIsApplying; }
	
	bool popPaintLayer();
	void resetPosition();
	
	ci::gl::Texture* paintMask();
	ci::Vec3f position() const { return mPosition; }
	
private:
	ci::Vec3f mPosition;
	TexturePaintState mState;
	ci::gl::Fbo* currentLayer() const;
	
	std::vector<ci::gl::Fbo*> mPaintLayers;
	
	bool mIsApplying;
	std::vector<ci::gl::Texture*> mBrushTextures;
	
	ci::gl::Fbo* createFbo();
	void drawOutputLayers();
		
	ci::gl::Fbo* mFlattenedOutputFbo;
	ci::gl::Fbo* mFlattenedHistoryFbo;
};

}