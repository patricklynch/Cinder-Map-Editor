#pragma once

#include "Input.h"

#include "cinder/Vector.h"

namespace ly {
	
class Editor;
	
static const ci::Vec2f kKeyRotationSpeed = ci::Vec2f( 50, 50 );

class EditorKeyCommands {
public:
	EditorKeyCommands( Editor* editor );
	virtual ~EditorKeyCommands();
	
	void onKeyDown( ci::app::KeyEvent );
	void update( const float deltaTime );
	
private:
	Editor* mEditor;
	ly::Input* mInput;
};

}