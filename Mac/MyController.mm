#import "MyController.h"

@implementation MyController

- (IBAction) listenToCubeSizeSlider: (NSSlider*) sender
{
	int value = [sender intValue];
	[CinderDrawing setCubeSize:value];
    [CinderDrawing setNeedsDisplay:YES]; // to show effect right away while sliding
}

@end
