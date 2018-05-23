//
//  MaschineThread.h
//  PacMaschine
//
//  Created by Antonio Malara on 23/05/2018.
//  Copyright © 2018 Antonio Malara. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MaschineThread : NSThread

+ (MaschineThread *)thread;

- (void)gotFrame:(NSData *)frameData;

@end
