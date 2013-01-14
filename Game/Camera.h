#pragma once

#include "Node.h"

#include "cinder/Camera.h"
#include "cinder/Ray.h"

namespace ly {

class Camera : public Node {
public:
	static Camera* get();
	virtual ~Camera();
	virtual void update( const float deltaTime );
	
	ci::CameraPersp& cinderCamera() { return mCinderCamera; }
	virtual ci::Vec3f globalPosition();
	
	void setRange( float near, float far );
	void setZoom( float zoom );
	void setAngle( float angleX );
	void setFov( float fov );
	
	float zoom() const { return mBody.position.z; }
	float angle() const { return mPivot.rotation.x; }
	
	ci::Ray rayIntoScene( ci::Vec2i screenPoint );
	
private:
	static Camera* sInstance;
	Camera();
	
	float mFov;
	ly::Node mBody;
	ly::Node mPivot;
	ci::CameraPersp mCinderCamera;
	
};

}