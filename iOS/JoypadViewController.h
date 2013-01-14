//
//  JoypadViewController.h
//  Fighting
//
//  Created by Patrick Lynch on 12/23/12.
//  Copyright (c) 2012 lynchdev. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface JoypadViewController : UIViewController {
	CGPoint mValue;
}

@property (nonatomic, retain ) IBOutlet UIView* joypad;
@property (nonatomic, readonly) CGPoint value;
@end
