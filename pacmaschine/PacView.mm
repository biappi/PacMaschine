//
//  PacView.m
//  PacMaschine
//
//  Created by Antonio Malara on 15/05/2018.
//  Copyright © 2018 Antonio Malara. All rights reserved.
//

#import "PacView.h"

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

@end


void want_a_frame(const uint8_t * data, int w, int h) {
	auto thread = BoiThread.thread;
	
	auto nsdata = [NSMutableData dataWithBytes:data length:w * h];
	
	auto srcBlock = data;
	auto dstByte  = (uint8_t *)nsdata.mutableBytes;

	
	for (int i = 0; i < (w * h / 8); i++) {
		auto block = *srcBlock++;

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

@end
