//
//  NIImageConversions.h
//  Macchina
//
//  Created by Antonio "Willy" Malara on 17/08/12.
//  Copyright (c) 2012 Antonio "Willy" Malara. All rights reserved.
//

#import "NIProtocolMessages.h"

static const int NIMaschineDisplaysWidth  = 255;
static const int NIMaschineDisplaysHeight =  64;

NIDisplayDrawMessage * TestImageDataMessage(void);
NSData * NI24BPPToST7529Data(uint16_t width, uint16_t height, const uint8_t * bitmap);
