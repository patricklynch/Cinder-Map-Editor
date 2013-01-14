//
//  JoypadViewController.m
//  Fighting
//
//  Created by Patrick Lynch on 12/23/12.
//  Copyright (c) 2012 lynchdev. All rights reserved.
//

#import "JoypadViewController.h"

@interface JoypadViewController ()
@end

@implementation JoypadViewController

@synthesize joypad;
@dynamic value;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
}

-(CGPoint) value
{
	return mValue;
}

-(void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch* touch = [touches anyObject];
	CGPoint loc = [touch locationInView:self.view];
	CGRect fr = self.joypad.frame;
	fr.origin.x = loc.x - fr.size.width * 0.5f;
	fr.origin.y = loc.y - fr.size.height * 0.5f;
	self.joypad.frame = fr;
}

-(void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch* touch = [touches anyObject];
	CGPoint loc = [touch locationInView:self.joypad];
	
	float x = loc.x / self.joypad.frame.size.width;
	float y = 1.0f - loc.y / self.joypad.frame.size.height;
	
	x -= 0.5f; x *= 2.0f;
	y -= 0.5f; y *= 2.0f;
	
	x = x > 1.0f ? 1.0f : x < -1.0f ? -1.0f : x;
	y = y > 1.0f ? 1.0f : y < -1.0f ? -1.0f : y;
	
	mValue = CGPointMake( x, y );
}

-(void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	mValue = CGPointMake( 0, 0 );
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
