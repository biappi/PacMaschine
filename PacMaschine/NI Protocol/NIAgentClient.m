//
//  NIAgentClient.m
//  Macchina
//
//  Created by Antonio "Willy" Malara on 16/08/12.
//  Copyright (c) 2012 Antonio "Willy" Malara. All rights reserved.
//

#import "NIAgentClient.h"
#import "NIMainHandlerClient.h"
#import "NIControllerRequestClient.h"
#import "NIControllerNotificationServer.h"
#import "NIImageConversions.h"

@interface NIAgentClient ()
@property (readonly) NIMainHandlerClient            * mainHandler;
@property (readonly) NIControllerRequestClient      * requestClient;
@property (readonly) NIControllerNotificationServer * notificationServer;
@end

@implementation NIAgentClient

- (id)init;
{
    if ((self = [super init]) == nil)
        return nil;
    
    return self;
}

- (BOOL)connectWithClientName:(NSString *)clientName;
{
    _mainHandler = [[NIMainHandlerClient alloc] initWithName:@"NIHWMainHandler"];
    
    NIDeviceConnectResponse * r = [self.mainHandler connectToControllerWithId:0x00000808
                                                                          boh:'NiMS'
                                                                   clientRole:'prmy'
                                                                   clientName:clientName];
    
    if (r.success == NO)
        return NO;
    
    _requestClient      = [[NIControllerRequestClient alloc]      initWithName:r.inPortName];
    _notificationServer = [[NIControllerNotificationServer alloc] initWithName:r.outPortName];
    _notificationServer.observer = self.notificationObserver;
    
    [_notificationServer scheduleInRunLoop:[NSRunLoop currentRunLoop]];
    [_requestClient setNotificationPortName:r.outPortName];
    
    return YES;
}

- (void)allLedsOff;
{
    [self setLedState:[NILedState new]];
}

- (void)setLedState:(NILedState *)ledState;
{
    NISetLedStateMessage * setLed = [NISetLedStateMessage new];
    setLed.state = ledState;
    [self.requestClient sendMessage:setLed];
}

- (void)blankLcds;
{
    NIDisplayDrawMessage * blank = [NIDisplayDrawMessage new];
    blank.sizeWidth  = NIMaschineDisplaysWidth;
    blank.sizeHeight = NIMaschineDisplaysHeight;
    blank.st7529EncodedImage = [NSMutableData dataWithLength:NIMaschineDisplaysWidth * NIMaschineDisplaysHeight / 3 * 2];
    
    [self.requestClient sendMessage:blank];
    
    blank.displayNumber = 1;
    [self.requestClient sendMessage:blank];
}

- (void)sendDrawMessage:(NIDisplayDrawMessage *)msg;
{
    [self.requestClient sendMessage:msg];
}

@end
