//
//  BoiThread.h
//  PacMaschine
//
//  Created by Antonio Malara on 23/05/2018.
//  Copyright © 2018 Antonio Malara. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "MaschineBoi.h"

@interface BoiThread : NSThread

@property(assign) id  deliverFrameTarget;
@property(assign) SEL deliverFrameAction;

+ (BoiThread *)thread;

- (void)buttonPress:(Buttons)button;
- (void)buttonRelease:(Buttons)button;

@end
