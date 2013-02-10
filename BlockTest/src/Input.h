#pragma once

#include "cinder/app/MouseEvent.h"
#include "cinder/app/KeyEvent.h"
#include "cinder/Vector.h"

#include <boost/function.hpp>

#include <vector>
#include <map>

namespace ly {
	
class MouseDrag;
	
class KeyListener_t {
public:
	boost::function<void (ci::app::KeyEvent)> callback;
	int keyCode;
};
	
class MouseListener_t {
public:
	boost::function<void (ci::app::MouseEvent)> callback;
	int keyCode;
};
	
typedef enum {
	MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE
} MouseButton;

class MouseDrag {
public:
	MouseDrag() : isActive( false ), isAltDown( false ), isControlDown( false ) {}
	bool isActive;
	ci::Vec2i origin;
	ci::Vec2i current;
	bool isAltDown;
	bool isControlDown;
	bool isShiftDown;
	bool isMetaDown;
	ci::Vec2i difference() const { return current - origin; }
	MouseButton mouseButton;
};
	
class Input {
public:
	static Input* get();
	
	void addListenerForKey( boost::function<void (ci::app::KeyEvent)> callback, int keyCode = 0 );
	void addListenerForMouseDown( boost::function<void (ci::app::MouseEvent)> callback );
	void addListenerMouseWheel( boost::function<void (float)> callback );
	
	void keyDown( ci::app::KeyEvent event );
	void keyUp( ci::app::KeyEvent event );
	void mouseMove( ci::app::MouseEvent event );
	void mouseDrag( ci::app::MouseEvent event );
	void mouseDown( ci::app::MouseEvent event );
	void mouseWheel( ci::app::MouseEvent event );
	void mouseUp( ci::app::MouseEvent event );
	void update( const float deltaTime );
	
	ci::Vec2i mousePosition() const;
	
	bool keyIsDown( int keyCode);
	bool mouseIsDown( MouseButton mouseButton );
	
	ci::Vec2i moveVelocity() const { return mMoveVelocity; }
	
	MouseDrag& getMouseDrag();
	
private:
	Input();
	~Input();
	
	bool mCursorPositionHasBeenSet;
	void activateDrag( ci::app::MouseEvent event );
	
	MouseButton buttonForEvent( ci::app::MouseEvent event );
	std::vector<KeyListener_t> mKeyListeners;
	std::vector<MouseListener_t> mMouseListeners;
	ci::Vec2i mLastMovePos;
	ci::Vec2i mMoveVelocity;
	ci::Vec2i mCurrentMovePos;
	
	MouseDrag mMouseDrag;
	
	std::map<int, bool> mKeyInputs;
	std::map<int, bool> mAllKeyCallbacks;
	std::map<MouseButton, bool> mMouseInputs;
	boost::function<void (float)> mMouseWheelCallback;
	
	static Input* sInstance;
};

}