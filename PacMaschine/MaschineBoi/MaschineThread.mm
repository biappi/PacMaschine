//
//  MaschineThread.m
//  PacMaschine
//
//  Created by Antonio Malara on 23/05/2018.
//  Copyright © 2018 Antonio Malara. All rights reserved.
//

#import "MaschineThread.h"
#import "BoiThread.h"
#import "MaschineBoi.h"
#import "NIAgentClient.h"

struct {
    NIMaschineButtonsLayout keycode;
    Buttons                 button;
} keyCodeToButtons[] = {
    { NIMaschineButton_GroupA, A_BUTTON     },
    { NIMaschineButton_Rec,    B_BUTTON     },
    { NIMaschineButton_GroupF, DOWN_BUTTON  },
    { NIMaschineButton_GroupB, UP_BUTTON    },
    { NIMaschineButton_GroupE, LEFT_BUTTON  },
    { NIMaschineButton_GroupG, RIGHT_BUTTON },
};

@interface MaschineThread () <NIControllerNotificationsObserver>
@end

@implementation MaschineThread
{
    NSRunLoop     * runLoop;
    NIAgentClient * maschine;
    NSMutableData * gameScreen;
    
    NIDisplayDrawMessage * drawMessage;
    NILedState           * ledState;
}

+ (MaschineThread *)thread {
    static dispatch_once_t onceToken;
    static MaschineThread * thread = nil;
    
    dispatch_once(&onceToken, ^{
        thread = [MaschineThread new];
    });
    
    return thread;
}

- (void)main {
    runLoop = NSRunLoop.currentRunLoop;
    
    maschine = [NIAgentClient new];
    maschine.notificationObserver = self;

    gameScreen = [NSMutableData dataWithLength:MaschineBitmapByteSizeFor(WIDTH, HEIGHT)];
    
    drawMessage = [NIDisplayDrawMessage new];
    drawMessage.sizeWidth  = WIDTH;
    drawMessage.sizeHeight = HEIGHT;
    drawMessage.displayNumber = 0;
    drawMessage.st7529EncodedImage = gameScreen;
    
    ledState = [NILedState new];
    [ledState setLed:NIMaschineLed_GroupB intensity:0x7f];
    [ledState setLed:NIMaschineLed_GroupE intensity:0x7f];
    [ledState setLed:NIMaschineLed_GroupF intensity:0x7f];
    [ledState setLed:NIMaschineLed_GroupG intensity:0x7f];
    
    if ([maschine connectWithClientName:@"PacMaschine"] == NO) {
        runLoop = nil;
        NSLog(@"Cannot connect to the Maschine software!");
    }
    else {
        [NSRunLoop.currentRunLoop run];
    }
}

- (void)perform:(void (^)())block {
    block();
}

- (void)gotFrame:(NSData *)frameData {
    [self performSelector:@selector(perform:) withObject:^{
        ConvertArduboyToMaschine((const uint8_t *)frameData.bytes,
                                 (uint8_t *)self->gameScreen.mutableBytes,
                                 WIDTH,
                                 HEIGHT);
        [self->maschine sendDrawMessage:self->drawMessage];
    }];
}

- (void)gotFocus {
    [maschine sendDrawMessage:drawMessage];
    [maschine setLedState:ledState];
}

- (void)buttonChanged:(NIMaschineButtonsLayout)button pressed:(BOOL)pressed {
    for (auto &i : keyCodeToButtons) {
        if (button == i.keycode) {
            if (pressed) {
                [BoiThread.thread buttonPress:i.button];
            }
            else {
                [BoiThread.thread buttonRelease:i.button];
            }
        }
    }
}

@end
