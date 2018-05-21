//
//  NIControllerNotificationServer.m
//  Macchina
//
//  Created by Antonio "Willy" Malara on 16/08/12.
//  Copyright (c) 2012 Antonio "Willy" Malara. All rights reserved.
//

#import "NIControllerNotificationServer.h"
#import "NIProtocolMessages.h"
#import "NIAgentClient.h"

@implementation NIControllerNotificationServer

- (NSData *)handleNIDeviceStateChangeMessage:(NIDeviceStateChangeMessage *)stateChange;
{
    return nil;
}

- (NSData *)handleNISetFocusMessage:(NISetFocusMessage *)setFocus;
{
    [self.observer gotFocus];
    return nil;
}

- (NSData *)handleNIButtonsChangedMessage:(NIButtonsChangedMessage *)msg;
{
    for (NIButtonsChangedEvent * ev in msg.events)
        [self.observer buttonChanged:ev.buttonId pressed:ev.on];
    
    return nil;
}

@end
