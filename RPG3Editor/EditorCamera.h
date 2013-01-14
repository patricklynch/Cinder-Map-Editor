#pragma once

#include "Camera.h"

namespace ly {

class EditorCamera {
public:
	EditorCamera();
	void reset();
	virtual void update( const float deltaTime );
	
private:
	ly::Camera* mCamera;
	ci::Vec3f mRotationStart;
	ci::Vec3f mPositionStart;
	float mZoomStart;
	
	float mDefaultZoom;
	ci::Vec3f mDefaultPosition;
	ci::Vec3f mDefaultRotation;
};

}