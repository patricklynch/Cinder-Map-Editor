#pragma once

#include "Input.h"

namespace ly {
	
class Editor;

class EditorKeyCommands {
public:
	EditorKeyCommands( Editor* editor );
	virtual ~EditorKeyCommands();
	
	void onKeyDown( ci::app::KeyEvent );
	
private:
	Editor* mEditor;
	ly::Input* mInput;
};

}