#import <Cocoa/Cocoa.h>

#include "EditorMode.h"

#import "TexturePalette.h"

@interface MyController : NSObject <TexturePaletteDelegate> {
	ly::EditorMode* mEditor;
	IBOutlet NSButton *btn0;
	IBOutlet NSButton *btn1;
	IBOutlet NSButton *btn2;
	IBOutlet NSButton *btn3;
}

-(IBAction) setMode:(NSButton*) button;
-(void) setEditor:(ly::EditorMode*)editor;
-(void) didSelectionLocation:(NSPoint)point;
-(IBAction) setElevation:(NSSlider*)slider;

@end