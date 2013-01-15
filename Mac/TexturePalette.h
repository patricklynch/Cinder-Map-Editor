//
//  TexturePalette.h
//  RPG3
//
//  Created by Patrick Lynch on 1/14/13.
//  Copyright (c) 2013 lynchdev. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@protocol TexturePaletteDelegate <NSObject>
-(void) didSelectionLocation:(NSPoint)point;
@end

@interface TexturePalette : NSImageView {
	IBOutlet id<TexturePaletteDelegate> delegate;
}
@end
