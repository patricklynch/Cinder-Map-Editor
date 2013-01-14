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

Input::Input() : mMouseDrag( new MouseDrag() ), mCursorPositionHasBeenSet( false )
{
	mCurrentMovePos = app::getWindowCenter();
	mLastMovePos = mCurrentMovePos;
	
	mMouseInputs[ Input::MOUSE_LEFT ] = false;
	mMouseInputs[ Input::MOUSE_RIGHT ] = false;
	mMouseInputs[ Input::MOUSE_MIDDLE ] = false;
}

Input::~Input()
{
	delete mMouseDrag;
}

void Input::keyDown( ci::app::KeyEvent event )
{
	mKeyInputs[ event.getCode() ] = true;
	
	for( std::vector<KeyListener>::iterator iter = mKeyListeners.begin(); iter != mKeyListeners.end(); ++iter)
		if ( iter->keyCode == event.getCode())
			iter->callback( event.getCode() );
}

void Input::addListenerForKey( boost::function<void (int)> callback, int keyCode)
{
	KeyListener listener;
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

MouseDrag* Input::mouseDrag(MouseButton mouseButton) const
{
	if ( mMouseDrag->isActive && mMouseDrag->mouseButton == mouseButton ) return mMouseDrag;
	else return NULL;
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
	mMouseDrag->isActive = true;
	mMouseDrag->isAltDown = event.isAltDown();
	mMouseDrag->isControlDown = event.isControlDown();
	mMouseDrag->current = event.getPos();
	mMouseDrag->mouseButton = buttonForEvent( event );
	mMouseInputs[ buttonForEvent( event ) ] = true;
}

void Input::mouseDown( ci::app::MouseEvent event )
{
	mMouseDrag->isActive = false; // not yet, until drag event comes
	mMouseDrag->origin = event.getPos();
	mMouseDrag->isAltDown = event.isAltDown();
	mMouseDrag->isControlDown = event.isControlDown();
	mMouseDrag->mouseButton = buttonForEvent( event );
	mMouseInputs[ buttonForEvent( event ) ] = true;
}

void Input::mouseUp( ci::app::MouseEvent event )
{
	mMouseDrag->isActive = false;
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

Input::MouseButton Input::buttonForEvent( ci::app::MouseEvent event )
{
	if (event.isLeft()) return Input::MOUSE_LEFT;
	else if (event.isMiddle()) return Input::MOUSE_MIDDLE;
	else return Input::MOUSE_RIGHT;
}
