#pragma once

#include "cinder/app/MouseEvent.h"
#include "cinder/app/KeyEvent.h"
#include "cinder/Vector.h"

#include <boost/function.hpp>

#include <vector>
#include <map>

namespace ly {
	
class MouseDrag;
	
class KeyListener {
public:
	boost::function<void (int)> callback;
	int keyCode;
};
	
class Input {
public:
	static Input* get();
	
	typedef enum {
		MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE
	} MouseButton;
	
	void addListenerForKey( boost::function<void (int)> callback, int keyCode );
	void keyDown( ci::app::KeyEvent event );
	void keyUp( ci::app::KeyEvent event );
	void mouseMove( ci::app::MouseEvent event );
	void mouseDrag( ci::app::MouseEvent event );
	void mouseDown( ci::app::MouseEvent event );
	void mouseUp( ci::app::MouseEvent event );
	void update( const float deltaTime );
	
	ci::Vec2i mousePosition() const;
	
	bool keyIsDown( int keyCode);
	bool mouseIsDown( MouseButton mouseButton);
	
	ci::Vec2i moveVelocity() const { return mMoveVelocity; }
	
	MouseDrag* mouseDrag(MouseButton mouseButton) const;
	
private:
	Input();
	~Input();
	
	bool mCursorPositionHasBeenSet;
	
	MouseButton buttonForEvent( ci::app::MouseEvent event );
	std::vector<KeyListener> mKeyListeners;
	ci::Vec2i mLastMovePos;
	ci::Vec2i mMoveVelocity;
	ci::Vec2i mCurrentMovePos;
	
	MouseDrag* mMouseDrag;
	
	std::map<int, bool> mKeyInputs;
	std::map<MouseButton, bool> mMouseInputs;
	
	static Input* sInstance;
};

class MouseDrag {
public:
	bool isActive;
	ci::Vec2i origin;
	ci::Vec2i current;
	bool isAltDown;
	bool isControlDown;
	ci::Vec2i difference() const { return current - origin; }
	Input::MouseButton mouseButton;
};

}