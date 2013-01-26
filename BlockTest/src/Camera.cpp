#include "Camera.h"

#include "cinder/app/App.h"

using namespace ci;
using namespace ci::app;

using namespace ly;

ly::Camera* ly::Camera::sInstance = NULL;

ly::Camera* ly::Camera::get()
{
	if ( !sInstance ) sInstance = new ly::Camera();
	return sInstance;
}

void ly::Camera::setZoom( float zoom )
{
	mBody.position.z = zoom;
}

void ly::Camera::setAngle( float angleX )
{
	mPivot.rotation.x = angleX;
}

void ly::Camera::setFov( float fov )
{
	mFov = fov;
	mCinderCamera.setPerspective( mFov, app::getWindowAspectRatio(), 0.1f, 4000.0f );
}

ci::Ray ly::Camera::rayIntoScene( ci::Vec2i screenPoint )
{
	float u = (float) screenPoint.x / (float) app::getWindowWidth();
	float v = (float) screenPoint.y / (float) app::getWindowHeight();
	Ray ray = mCinderCamera.generateRay(u , 1.0f - v, mCinderCamera.getAspectRatio() );
	return ray;
}

ly::Camera::Camera()
{
	mFov = 45.0f;
	mCinderCamera.setPerspective( mFov, app::getWindowAspectRatio(), 1.0f, 4000.0f );
	
	mPivot.rotation.x	= -13.0f;
	mBody.position.z	= 30.0f;
	
	mBody.parent = &mPivot;
	mPivot.parent = this;
}

ly::Camera::~Camera() {}

ci::Vec3f ly::Camera::globalPosition()
{
	return mBody.globalPosition();
}

void ly::Camera::update( const float deltaTime )
{	
	Node::update( deltaTime );
	mPivot.update( deltaTime );
	mBody.update( deltaTime );
	
	mCinderCamera.setWorldUp( Vec3f::yAxis() );
	mCinderCamera.setEyePoint( mBody.globalPosition() );
	mCinderCamera.setOrientation( Quatf( mBody.transform() ) );
	
}