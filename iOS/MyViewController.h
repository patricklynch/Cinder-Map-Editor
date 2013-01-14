//
//  MyViewController.h
//  CCGLTouchBasic example
//
//  Created by Matthieu Savary on 09/09/11.
//  Copyright (c) 2011 SMALLAB.ORG. All rights reserved.
//
//  More info on the CCGLTouch project >> http://cocoacid.org/ios/
//  License & disclaimer >> see license.txt file included in the distribution package
//

#import "CCGLTouchViewController.h"
#import "JoypadViewController.h"
#import "MyCCGLView.h"

#include "Game.h"
#include "InputButtons.h"

#include <vector>

@interface MyViewController : CCGLTouchViewController <MyCCGLViewDelegate> {
    MyCCGLView *ccglView;
	float mLastTimestamp;
	float mUpdateRate;
	ly::Game* mGame;
}

-(IBAction) onButtonDown: (UIButton*)button;
-(IBAction) onButtonUp: (UIButton*)button;

@property (retain, nonatomic) IBOutletCollection(UIButton) NSArray *buttons;

@property (retain, nonatomic) IBOutlet JoypadViewController *joypadVC;

- (void) update:(CADisplayLink*)displayLink;
- (void) setCCGLView:(CCGLTouchView *)view;
-(std::vector<ly::Touch>) prepTouches:(NSSet*)touches;

@end
