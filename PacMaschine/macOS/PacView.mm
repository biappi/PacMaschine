//
//  PacView.m
//  PacMaschine
//
//  Created by Antonio Malara on 15/05/2018.
//  Copyright © 2018 Antonio Malara. All rights reserved.
//

#import "PacView.h"
#import "MaschineBoi.h"
#import "BoiThread.h"

struct {
    int     keycode;
    Buttons button;
} keyCodeToButtons[] = {
    {   0, A_BUTTON     }, // 'a'
    {   1, B_BUTTON     }, // 's'
    { 125, DOWN_BUTTON  }, // arrows
    { 126, UP_BUTTON    },
    { 123, LEFT_BUTTON  },
    { 124, RIGHT_BUTTON },
};

@implementation PacView
{
    NSMutableData * frameBackingData;
    CGImageRef      frameImage;
    ButtonsState    buttonsState;
}

- (void)awakeFromNib {
    frameBackingData = [NSMutableData dataWithLength:WIDTH * HEIGHT];
    
    {
        auto dataProvider = CGDataProviderCreateWithCFData((CFDataRef)frameBackingData);
        auto colorSpace   = CGColorSpaceCreateDeviceGray();
        
        frameImage = CGImageCreate(WIDTH,
                                   HEIGHT,
                                   8,
                                   8,
                                   WIDTH,
                                   colorSpace,
                                   kCGImageAlphaNone,
                                   dataProvider,
                                   nil,
                                   NO,
                                   kCGRenderingIntentDefault);
        
        CGColorSpaceRelease(colorSpace);
        CGDataProviderRelease(dataProvider);
    }
    
    BoiThread.thread.deliverFrameTarget = self;
    BoiThread.thread.deliverFrameAction = @selector(gotFrame:);
    
    [BoiThread.thread start];
    [self becomeFirstResponder];
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (void)gotFrame:(NSData *)frameData {
    if (frameData.length < frameBackingData.length)
        return;
    
    memcpy(frameBackingData.mutableBytes,
           frameData.bytes,
           frameBackingData.length);
    
    [self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)rect {
    CGContextDrawImage((CGContextRef)NSGraphicsContext.currentContext.graphicsPort,
                       self.bounds,
                       frameImage);
}

- (void)keyDown:(NSEvent *)event {
    if (event.isARepeat)
        return;
    
    for (auto &i : keyCodeToButtons) {
        if (event.keyCode == i.keycode)
            buttonsState.press(i.button);
    }
}

- (void)keyUp:(NSEvent *)event {
    if (event.isARepeat)
        return;
    
    for (auto &i : keyCodeToButtons) {
        if (event.keyCode == i.keycode)
            buttonsState.release(i.button);
    }
}

@end
