#import <Cocoa/Cocoa.h>

@interface MyController : NSObject {
	IBOutlet id CinderDrawing;
}

- (IBAction) listenToCubeSizeSlider: (NSSlider*) sender;

@end