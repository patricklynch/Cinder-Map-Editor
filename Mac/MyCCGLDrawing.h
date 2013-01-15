#include "CCGLView.h"

#import "MyController.h"

#include "cinder/Camera.h"
#include "cinder/MayaCamUI.h"

#include "Game.h"
#include "EditorMode.h"

@interface MyCCGLDrawing : CCGLView
{
	MayaCamUI mMayaCam;
	int mCubeSize;
	ly::Game* mGame;
	ly::EditorMode* mEditor;
	float mUpdateRate;
	NSDate* mLastDate;
	IBOutlet MyController* controller;
}

- (void) update;
- (int)prepMouseEventModifiers:(NSEvent *)evt;
- (int)prepKeyEventModifiers:(NSEvent *)evt;

- (void)mouseUp:(NSEvent*)theEvent initiator:(int) initiator;
- (void)mouseDown:(NSEvent*)theEvent initiator:(int) initiator;
- (void)mouseDragged:(NSEvent*)theEvent initiator:(int) initiator;
- (void)mouseMoved:(NSEvent*)theEvent initiator:(int) initiator;

@end
