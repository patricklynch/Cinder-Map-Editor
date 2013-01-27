#include "EditorCamera.h"
#include "Input.h"

#include <boost/bind.hpp>

using namespace ly;

using namespace ci;
using namespace ci::app;


EditorCamera::EditorCamera()
{
	mCamera = ly::Camera::get();
	mDefaultZoom = mCamera->zoom();
	mDefaultPosition = mCamera->position;
	mDefaultRotation = mCamera->rotation;
	
	//Input::get()->addListenerMouseWheel( boost::bind( &EditorCamera::onMouseWheel, this ) );
}

void EditorCamera::onMouseWheel( float increment )
{
	mCamera->setZoom( mCamera->zoom() + increment );
}

void EditorCamera::reset()
{
	mCamera->setZoom( mDefaultZoom );
	mCamera->position = mDefaultPosition;
	mCamera->rotation = mDefaultRotation;
}

void EditorCamera::update( const float deltaTime )
{
	MouseDrag* drag;
	Input* input = Input::get();
	
	drag = input->mouseDrag( Input::MOUSE_MIDDLE );
	if (drag != NULL) {
		if ( drag->isAltDown ) {
			mCamera->rotation.y = mRotationStart.y - drag->difference().x * 0.17f;
			mCamera->rotation.x = mRotationStart.x - drag->difference().y * 0.17f;
		}
		else {
			float targetX = drag->difference().x * 0.001f * mCamera->zoom();
			float targetZ = drag->difference().y * 0.001f * mCamera->zoom();
			Vec3f tr = mCamera->transform().transformVec( Vec3f( targetX, 0.0f, targetZ ) );
			mCamera->position.x = mPositionStart.x - tr.x;
			mCamera->position.z = mPositionStart.z - tr.z;
		}
	}
	else {
		mRotationStart.x = mCamera->rotation.x;
		mRotationStart.y = mCamera->rotation.y;
		mPositionStart = mCamera->position;
	}
	
	drag = input->mouseDrag( Input::MOUSE_RIGHT );
	if (drag != NULL && drag->isAltDown) mCamera->setZoom( mZoomStart - drag->difference().x * 0.07f );
	else mZoomStart = mCamera->zoom();
}