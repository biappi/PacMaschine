//
//  BoiThread.m
//  PacMaschine
//
//  Created by Antonio Malara on 23/05/2018.
//  Copyright © 2018 Antonio Malara. All rights reserved.
//

#import "MaschineBoi.h"
#import "BoiThread.h"

uint8_t      globalScreenBuffer[WIDTH * HEIGHT];
double       globalFramerate = 30;
ButtonsState globalButtonsState;
ButtonsState globalPreviousButtonState;

@implementation BoiThread

+ (BoiThread *)thread {
    static BoiThread * thread = nil;
    static dispatch_once_t onceToken;
    
    dispatch_once(&onceToken, ^{
        thread = [BoiThread new];
    });
    
    return thread;
}

- (void)main {
    setup();
    [self performSelector:@selector(loop) withObject:nil afterDelay:0];
    [NSRunLoop.currentRunLoop run];
}

- (void)loop {
    @autoreleasepool {
        loop();
    }
    [self performSelector:@selector(loop) withObject:nil afterDelay:0];
}

- (void)stopLoop {
    CFRunLoopStop(CFRunLoopGetCurrent());
}

@end

void want_a_frame(const uint8_t * data, int w, int h) {
    @autoreleasepool {
        auto thread  = BoiThread.thread;
        auto nsdata  = [NSMutableData dataWithBytes:data length:w * h];
        auto dstByte = (uint8_t *)nsdata.mutableBytes;
        
        ConvertArduboyBitmap(data, dstByte, w, h);
        
        [thread.deliverFrameTarget performSelectorOnMainThread:thread.deliverFrameAction
                                                    withObject:nsdata
                                                 waitUntilDone:NO];
        
        [thread performSelector:@selector(stopLoop) withObject:nil afterDelay:1/globalFramerate];
        [NSRunLoop.currentRunLoop run];
    }
}
