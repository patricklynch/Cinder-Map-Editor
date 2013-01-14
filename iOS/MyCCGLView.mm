//
//  MyCCGLView.mm
//  CCGLTouchBasic example
//
//  Created by Matthieu Savary on 09/09/11.
//  Copyright (c) 2011 SMALLAB.ORG. All rights reserved.
//
//  More info on the CCGLTouch project >> http://cocoacid.org/ios/
//  License & disclaimer >> see license.txt file included in the distribution package
//
//  Latest revision on 10/06/12.
//

#import "MyCCGLView.h"

@implementation MyCCGLView

@synthesize delegate;

- (void)prepareSettings
{
    [self enableAntiAliasing];
    frameRate = 30;
	[self enableMultiTouch:YES];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	[delegate touchesBegan:touches withEvent:event];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	[delegate touchesMoved:touches withEvent:event];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	[delegate touchesEnded:touches withEvent:event];
}

- (void)draw
{	
	[self.delegate onDraw];
}

@end
