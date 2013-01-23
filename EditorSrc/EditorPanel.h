#pragma once

#include "cinder/params/Params.h"

namespace ly {
	
enum {
	EditorPaintSelection2,
	EditorPaintTexture2,
	EditorPaintElevation2,
	EditorPaintObject2
};

class EditorPanel {
public:
	EditorPanel();
	virtual ~EditorPanel();
	
	void draw();
	void update( const float deltaTime );
	
	void test();
	
private:
	float mElevation;
	int mTextureIndex;
	int mCurrentTool;
	bool mTexturePaint;
	ci::params::InterfaceGl mParams;
	
};

}