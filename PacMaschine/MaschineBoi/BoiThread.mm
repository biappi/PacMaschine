//
//  BoiThread.m
//  PacMaschine
//
//  Created by Antonio Malara on 23/05/2018.
//  Copyright © 2018 Antonio Malara. All rights reserved.
//

#import "MaschineBoi.h"
#import "BoiThread.h"

uint8_t          globalScreenBuffer[WIDTH * HEIGHT];
double           globalFramerate = 30;
GameButtonsState globalButtonsState;

@implementation BoiThread
{
    NSRunLoop * runLoop;
}

+ (BoiThread *)thread {
    static BoiThread * thread = nil;
    static dispatch_once_t onceToken;
    
    dispatch_once(&onceToken, ^{
        thread = [BoiThread new];
    });
    
    return thread;
}

- (void)main {
    runLoop = NSRunLoop.currentRunLoop;
    
    setup();
    [self performSelector:@selector(loop) withObject:nil afterDelay:0];
    [runLoop run];
}

- (void)loop {
    @autoreleasepool {
        globalButtonsState.didFrame();
        loop();
    }
    [self performSelector:@selector(loop) withObject:nil afterDelay:0];
}

- (void)stopLoop {
    CFRunLoopStop(CFRunLoopGetCurrent());
}

- (void)perform:(void (^)())block {
    block();
}

- (void)buttonPress:(Buttons)button {
    [self performSelector:@selector(perform:) withObject:[^{
        globalButtonsState.press(button);
    } copy]];
}

- (void)buttonRelease:(Buttons)button {
    [self performSelector:@selector(perform:) withObject:[^{
        globalButtonsState.release(button);
    } copy]];
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
