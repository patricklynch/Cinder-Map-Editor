// Undo#include "EditorKeyCommands.h"
#include "Editor.h"
#include "EditorCommandQueue.h"
#include "EditorState.h"
#include "Camera.h"

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

void EditorKeyCommands::update( const float deltaTime )
{
	if ( mInput->keyIsDown( KeyEvent::KEY_LEFT ) ) {
		mEditor->mCamera->rotation.y += kKeyRotationSpeed.x * deltaTime;
	}
	else if ( mInput->keyIsDown( KeyEvent::KEY_RIGHT ) ) {
		mEditor->mCamera->rotation.y -= kKeyRotationSpeed.x * deltaTime;
	}
	
	if ( mInput->keyIsDown( KeyEvent::KEY_UP ) ) {
		float angle = mEditor->mCamera->angle() + kKeyRotationSpeed.y * deltaTime;
		mEditor->mCamera->setAngle( angle );
	}
	else if ( mInput->keyIsDown( KeyEvent::KEY_DOWN ) ) {
		float angle = mEditor->mCamera->angle() - kKeyRotationSpeed.y * deltaTime;
		mEditor->mCamera->setAngle( angle );
	}
}

void EditorKeyCommands::onKeyDown( ci::app::KeyEvent event )
{
	int code = event.getCode();
#if _WINDOWS
	bool meta = event.isControlDown();
#else
	bool meta = event.isMetaDown();
#endif
	bool shift = event.isShiftDown();
	bool alt  = event.isAltDown();
	EditorState& state = mEditor->mState;

	if ( meta ) {
		
		// Undo & Redo
		if ( !shift && code == KeyEvent::KEY_z )  {
			mEditor->mCommandQueue.undo();
		}  else if ( code == KeyEvent::KEY_y )  {
			mEditor->mCommandQueue.redo();
		} else if ( shift && code == KeyEvent::KEY_z )  {
			mEditor->mCommandQueue.redo();
		}
	}
	
	else if ( !meta ) {
		
		int numValue = -1;
		switch( code ) {
			case KeyEvent::KEY_1: numValue = 1; break;
			case KeyEvent::KEY_2: numValue = 2; break;
			case KeyEvent::KEY_3: numValue = 3; break;
			case KeyEvent::KEY_4: numValue = 4; break;
			case KeyEvent::KEY_5: numValue = 5; break;
			case KeyEvent::KEY_6: numValue = 6; break;
			case KeyEvent::KEY_7: numValue = 7; break;
			case KeyEvent::KEY_8: numValue = 8; break;
			case KeyEvent::KEY_9: numValue = 9; break;
			case KeyEvent::KEY_0: numValue = 0; break;
		}
		
		if ( numValue > 0 ) {
			if ( state.mode == MODE_PAINT_TEXTURE ) {
				state.texturePaint.textureIndex = numValue-1;
			}
			else if ( state.mode == MODE_PAINT_ELEVATION ) {
				state.elevationSelection = numValue-1;
			}
		}
		
		// Set paint mode brush size
		else if ( code == KeyEvent::KEY_LEFTBRACKET ) {
			state.modifySelection( -1 );
		} else if ( code == KeyEvent::KEY_RIGHTBRACKET ) {
			state.modifySelection( 1 );
		}
		
		// Set elevation
		else if ( code == KeyEvent::KEY_PLUS || code == KeyEvent::KEY_EQUALS ) {
			state.modifyValue( 1 );
		}
		else if ( code == KeyEvent::KEY_MINUS || code == KeyEvent::KEY_KP_MINUS ) {
			state.modifyValue( -1 );
		}
		
		// Show/hide grid
		else if ( code == KeyEvent::KEY_g ) {
			state.showGrid = !state.showGrid;
		}
		
		// Clear everything off the map
		else if ( code == KeyEvent::KEY_SPACE ) {
			mEditor->resetElevation();
		}
		
		// Set to texture paint mode
		else if ( code == KeyEvent::KEY_p ) {
			state.mode = MODE_PAINT_TEXTURE;
		}
		
		// Set to elevation paint mode
		else if ( code == KeyEvent::KEY_e ) {
			state.mode = MODE_PAINT_ELEVATION;
		}
	}
	
}