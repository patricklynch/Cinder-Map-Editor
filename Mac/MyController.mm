#import "MyController.h""
#include "EditorTypes.h"

@implementation MyController

- (id)init {
    self = [super init];
    if (self) {
    }
    return self;
}

-(void) setEditor:(ly::EditorMode*)editor
{
	mEditor = editor;
}

-(void) didSelectionLocation:(NSPoint)point
{
	mEditor->setCurrentTextureLoc( point.x, point.y );
}

-(IBAction) setElevation:(NSSlider*)slider
{
	mEditor->setCurrentElevation( [slider floatValue] );
}

-(IBAction) setMode:(NSButton*) button
{
	NSArray* toolButtons = [NSArray arrayWithObjects:btn0, btn1, btn2, btn3, nil ];
	mEditor->setMode( button.tag );
	for( NSButton* btn in toolButtons ) {
		if ( btn.tag != button.tag )
			[btn setState:NSOffState];
	}
}

-(IBAction) undo:(id) sender
{
	mEditor->undo();
}

-(IBAction) redo:(id) sender
{
	mEditor->redo();
}

@end
