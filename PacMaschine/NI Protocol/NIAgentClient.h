//
//  NIAgentClient.h
//  Macchina
//
//  Created by Antonio "Willy" Malara on 16/08/12.
//  Copyright (c) 2012 Antonio "Willy" Malara. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NIProtocolMessages.h"
#import "NIMaschineLayouts.h"

@protocol NIControllerNotificationsObserver <NSObject>
- (void)gotFocus;
- (void)buttonChanged:(NIMaschineButtonsLayout)button pressed:(BOOL)pressed;
@end

@interface NIAgentClient : NSObject

@property(nonatomic, weak) id<NIControllerNotificationsObserver> notificationObserver;

- (void)connect;

- (void)allLedsOff;
- (void)setLedState:(NILedState *)ledState;
- (void)blankLcds;
- (void)sendDrawMessage:(NIDisplayDrawMessage *)msg;

@end
