#include "Input.h"
	
#include "cinder/app/App.h"

#include <algorithm>

using namespace ly;
using namespace ci;
using namespace ci::app;

Input* Input::sInstance = 0;

Input* Input::get()
{
	if ( sInstance == 0)
		sInstance = new Input();
	return sInstance;
}

Input::Input() : mCursorPositionHasBeenSet( false ), mMouseWheelCallback( NULL )
{
	mCurrentMovePos = Vec2i(0,0);
	mLastMovePos = mCurrentMovePos;
	
	mMouseInputs[ MOUSE_LEFT ] = false;
	mMouseInputs[ MOUSE_RIGHT ] = false;
	mMouseInputs[ MOUSE_MIDDLE ] = false;
}

Input::~Input() {}

void Input::keyDown( ci::app::KeyEvent event )
{
	mKeyInputs[ event.getCode() ] = true;
	
	for( std::vector<KeyListener_t>::iterator iter = mKeyListeners.begin(); iter != mKeyListeners.end(); ++iter) {
		if ( iter->keyCode == event.getCode() || iter->keyCode == 0 ) {
			iter->callback( event );
		}
	}
}

void Input::addListenerForKey( boost::function<void (ci::app::KeyEvent)> callback, int keyCode )
{
	KeyListener_t listener;
	listener.callback = callback;
	listener.keyCode = keyCode;
	mKeyListeners.push_back( listener );
}

void Input::update( const float deltaTime )
{
	if( !mCursorPositionHasBeenSet ) {
		mCursorPositionHasBeenSet = true;
		mMoveVelocity = Vec2i::zero();
	}
	else {
		mMoveVelocity = mCurrentMovePos - mLastMovePos;
		mLastMovePos = mCurrentMovePos;
	}
}

ci::Vec2i Input::mousePosition() const
{
	return mCurrentMovePos;
}

MouseDrag& Input::getMouseDrag()
{
	return mMouseDrag;
}

void Input::keyUp( ci::app::KeyEvent event )
{
	mKeyInputs[ event.getCode() ] = false;
}

void Input::mouseMove( ci::app::MouseEvent event)
{
	mCurrentMovePos = event.getPos();
}

void Input::mouseDrag( ci::app::MouseEvent event )
{
	mCurrentMovePos = event.getPos();
	mMouseDrag.isActive = true;
	mMouseDrag.isAltDown = event.isAltDown();
	mMouseDrag.isControlDown = event.isControlDown();
	mMouseDrag.isShiftDown = event.isShiftDown();
	mMouseDrag.current = event.getPos();
	mMouseDrag.mouseButton = buttonForEvent( event );
	mMouseInputs[ buttonForEvent( event ) ] = true;
}

void Input::mouseWheel( ci::app::MouseEvent event )
{
	if ( mMouseWheelCallback != NULL )
		mMouseWheelCallback( event.getWheelIncrement() );
}

void Input::addListenerMouseWheel( boost::function<void (float)> callback )
{
	mMouseWheelCallback = callback;
}

void Input::mouseDown( ci::app::MouseEvent event )
{
	mMouseDrag.isActive = true;
	mMouseDrag.origin = event.getPos();
	mCurrentMovePos = event.getPos();
	mMouseDrag.current = event.getPos();
	mMouseDrag.isAltDown = event.isAltDown();
	mMouseDrag.isControlDown = event.isControlDown();
	mMouseDrag.isShiftDown = event.isShiftDown();
	mMouseDrag.mouseButton = buttonForEvent( event );
	
	mMouseInputs[ buttonForEvent( event ) ] = true;
}

void Input::mouseUp( ci::app::MouseEvent event )
{
	mMouseDrag.isActive = false;
	mMouseInputs[ buttonForEvent( event ) ] = false;
}

bool Input::keyIsDown( int keyCode)
{
	return mKeyInputs[ keyCode ];
}

bool Input::mouseIsDown( MouseButton mouseButton )
{
	return mMouseInputs[ mouseButton ];
}

MouseButton Input::buttonForEvent( ci::app::MouseEvent event )
{
	if (event.isLeft()) return MOUSE_LEFT;
	else if (event.isRight()) return MOUSE_RIGHT;
	else return MOUSE_MIDDLE;
}
