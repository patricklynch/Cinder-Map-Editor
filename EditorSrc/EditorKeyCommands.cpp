// Undo#include "EditorKeyCommands.h"
#include "Editor.h"
#include "EditorCommandQueue.h"

#include "cinder/app/App.h"

#include <boost/bind.hpp>

using namespace ly;

using namespace ci;
using namespace ci::app;

EditorKeyCommands::EditorKeyCommands( Editor* editor ) : mEditor( editor )
{
	mInput = Input::get();
	
	mInput->addListenerForKey( boost::bind( &EditorKeyCommands::onKeyDown, this, _1 ) );
}

EditorKeyCommands::~EditorKeyCommands() {}

void EditorKeyCommands::onKeyDown( ci::app::KeyEvent event )
{
	int code = event.getCode();
	bool meta = event.isMetaDown();
	bool ctrl = event.isControlDown();
	bool shift = event.isShiftDown();
	bool alt  = event.isAltDown();

	if ( meta ) {
		
		// Undo & Redo
		if ( code == KeyEvent::KEY_z )  {
			if ( !shift  ) {
				mEditor->mCommandQueue.undo();
				return;
			} else if ( shift ) {
				mEditor->mCommandQueue.redo();
				return;
			}
		}
	}
	
	else if ( !meta ) {
		
		// Brush Size
		if ( code == KeyEvent::KEY_LEFTBRACKET ) {
			mEditor->mState.brushSize--;
			return;
		} else if ( code == KeyEvent::KEY_RIGHTBRACKET ) {
			mEditor->mState.brushSize++;
			return;
		}
		
		// Elevation
		else if ( code == KeyEvent::KEY_PLUS || code == KeyEvent::KEY_EQUALS ) {
			mEditor->mState.targetElevation++;
			return;
		}
		else if ( code == KeyEvent::KEY_MINUS || code == KeyEvent::KEY_KP_MINUS ) {
			mEditor->mState.targetElevation--;
			return;
		}
		
		// Grid
		else if ( code == KeyEvent::KEY_g ) {
			mEditor->mState.showGrid = !mEditor->mState.showGrid;
		}
	}
	
}