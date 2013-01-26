#import <Cocoa/Cocoa.h>

#include "Editor.h"

#import "TexturePalette.h"

@interface MyController : NSObject <TexturePaletteDelegate> {
	ly::Editor* mEditor;
	IBOutlet NSButton *btn0;
	IBOutlet NSButton *btn1;
	IBOutlet NSButton *btn2;
	IBOutlet NSButton *btn3;
}

-(IBAction) setMode:(NSButton*) button;
-(void) setEditor:(ly::Editor*)editor;
-(void) didSelectionLocation:(NSPoint)point;
-(IBAction) setElevation:(NSSlider*)slider;

@end