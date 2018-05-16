//
//  PacView.m
//  PacMaschine
//
//  Created by Antonio Malara on 15/05/2018.
//  Copyright © 2018 Antonio Malara. All rights reserved.
//

#import "PacView.h"

enum Buttons {
    UP_BUTTON    = 0x01,
    DOWN_BUTTON  = 0x02,
    RIGHT_BUTTON = 0x04,
    LEFT_BUTTON  = 0x08,
    A_BUTTON     = 0x10,
    B_BUTTON     = 0x20,
    
};

struct ButtonsState;

extern ButtonsState globalButtonsState;
extern ButtonsState globalPreviousButtonState;

class ButtonsState {
    
private:
    
    uint8_t state = 0x00;
    
    void setState(uint8_t newValue) {
        state = newValue;
        
        globalPreviousButtonState = globalPreviousButtonState;
        globalButtonsState = *this;
        
    }
    
public:
    
    void press(Buttons button) {
        setState(state |= button);
    }
    
    void release(Buttons button) {
        setState(state &= ~(button));
    }
    
    bool pressed(Buttons button) {
        return !!(state & button);
    }
};


struct {
    int     keycode;
    Buttons button;
} keyCodeToButtons[] = {
    {   0, A_BUTTON     }, // 'a'
    {   1, A_BUTTON     }, // 's'
    { 125, DOWN_BUTTON  }, // arrows
    { 126, UP_BUTTON    },
    { 123, LEFT_BUTTON  },
    { 124, RIGHT_BUTTON },
};

extern const int WIDTH;
extern const int HEIGHT;

void setup();
void loop();

@interface BoiThread : NSThread
@property(assign) id  deliverFrameTarget;
@property(assign) SEL deliverFrameAction;
@end

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
    
    while (1)
        loop();
}

- (void)loop {
    loop();
    [self performSelector:@selector(loop) withObject:nil afterDelay:0];
}

@end

void want_a_frame(const uint8_t * data, int w, int h) {
    auto thread  = BoiThread.thread;
    auto nsdata  = [NSMutableData dataWithBytes:data length:w * h];
    auto dstByte = (uint8_t *)nsdata.mutableBytes;
    
    for (int i = 0; i < (w * h / 8); i++) {
        auto block = data[i];
        
        for (int j = 0; j < 8; j++) {
            auto x =  i % WIDTH;
            auto y = (i / WIDTH) * 8 + j;
            
            auto bit = block >> j;
            dstByte[y * WIDTH + x] = ((bit & 0b00000001) == 0b00000001) ? 0xff : 0x00;
        }
    }
    
    [thread.deliverFrameTarget performSelectorOnMainThread:thread.deliverFrameAction
                                                withObject:nsdata
                                             waitUntilDone:NO];
}

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
