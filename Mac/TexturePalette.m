//
//  TexturePalette.m
//  RPG3
//
//  Created by Patrick Lynch on 1/14/13.
//  Copyright (c) 2013 lynchdev. All rights reserved.
//

#import "TexturePalette.h"

@implementation TexturePalette

-(void) mouseDown:(NSEvent *)theEvent
{
	NSPoint curPoint = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	curPoint.x = curPoint.x / self.frame.size.width;
	curPoint.y = curPoint.y / self.frame.size.height;
	curPoint.x = floorf(curPoint.x * 8.0f);
	curPoint.y = floorf(curPoint.y * 8.0f);
	[delegate didSelectionLocation:curPoint];
}

@end
