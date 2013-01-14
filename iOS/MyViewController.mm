//
//  MyViewController.mm
//  CCGLTouchBasic example
//
//  Created by Matthieu Savary on 09/09/11.
//  Copyright (c) 2011 SMALLAB.ORG. All rights reserved.
//
//  More info on the CCGLTouch project >> http://cocoacid.org/ios/
//  License & disclaimer >> see license.txt file included in the distribution package
//

#import "MyViewController.h"

@implementation MyViewController
@synthesize buttons;
@synthesize joypadVC;

- (void)setCCGLView:(MyCCGLView *)view
{
    ccglView = view;
	ccglView.delegate = self;
	
	mGame = ly::Game::create( ccglView.frame.size.width, ccglView.frame.size.height );
	
	for( UIButton* btn in buttons) {
		[btn addTarget:self action:@selector( onButtonDown: ) forControlEvents:UIControlEventTouchDown ];
		[btn addTarget:self action:@selector( onButtonDown: ) forControlEvents:UIControlEventTouchDragEnter ];
		[btn addTarget:self action:@selector( onButtonUp: ) forControlEvents:UIControlEventTouchUpInside ];
		[btn addTarget:self action:@selector( onButtonUp: ) forControlEvents:UIControlEventTouchDragExit ];
	}
	
	mUpdateRate = 1.0f / 60.0f;
    CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update:)];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	mLastTimestamp = 0.0f;
}

-(IBAction) onButtonDown: (UIButton*)button
{
	mGame->onButtonDown( button.tag );
}

-(IBAction) onButtonUp: (UIButton*)button
{
	mGame->onButtonUp( button.tag );
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
}

-(std::vector<ly::Touch>) prepTouches:(NSSet*)touches
{
	std::vector<ly::Touch> lyTouches;
	for( UITouch* touch in touches ) {
		CGPoint touchLoc = [touch locationInView:ccglView];
		CGPoint touchLocPrev = [touch previousLocationInView:ccglView];
		ly::Touch lyTouch;
		lyTouch.position = Vec2i( touchLoc.x, touchLoc.y );
		lyTouch.prevPosition = Vec2i( touchLocPrev.x, touchLocPrev.y );
		lyTouches.push_back( lyTouch );
	}
	return lyTouches;
}

-(void) update:(CADisplayLink*)displayLink
{
	float currentTime = (float)displayLink.timestamp;
    float deltaTime = currentTime - mLastTimestamp;
	mLastTimestamp = currentTime;
	
	mGame->update( deltaTime );
	mGame->onJoypadInput( 0, joypadVC.value.x, joypadVC.value.y );
}

-(void) onDraw
{
	mGame->draw();
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}
@end