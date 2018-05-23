//
//  MaschineBoi.h
//  pacmaschine
//
//  Created by Antonio Malara on 16/05/2018.
//  Copyright © 2018 Antonio Malara. All rights reserved.
//

#ifndef MaschineBoi_h
#define MaschineBoi_h

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

class GameButtonsState;

static const int WIDTH  = 255;
static const int HEIGHT =  64;

extern uint8_t          globalScreenBuffer[];
extern double           globalFramerate;
extern GameButtonsState globalButtonsState;

void want_a_frame(const uint8_t * data, int w, int h);

// - //

enum Colors {
    BLACK = 0,
    WHITE = 1,
};

enum Buttons {
    UP_BUTTON    = 0x01,
    DOWN_BUTTON  = 0x02,
    RIGHT_BUTTON = 0x04,
    LEFT_BUTTON  = 0x08,
    A_BUTTON     = 0x10,
    B_BUTTON     = 0x20,
};

// - //

class ButtonsState {
    
private:
    uint8_t state = 0x00;
    
public:
    void press(Buttons button) {
        state |= button;
    }
    
    void release(Buttons button) {
        state &= ~(button);
    }
    
    bool pressed(Buttons button) {
        return !!(state & button);
    }
};

class GameButtonsState {
    
private:
    ButtonsState previous;
    ButtonsState current;
    ButtonsState next;
    
public:
    void press(Buttons button) {
        next.press(button);
    }
    
    void release(Buttons button) {
        next.release(button);
    }

    void poll() {
        current = next;
    }
    
    void didFrame() {
        previous = current;
    }
    
    bool pressed(Buttons button) {
        return current.pressed(button);
    }
  
    bool justPressed(Buttons button) {
        return current.pressed(button) && !previous.pressed(button);
    }
};

// - //

inline void ConvertArduboyBitmap(const uint8_t * src, uint8_t * dst, int w, int h) {
    for (int i = 0; i < (w * h / 8); i++) {
        auto block = src[i];
        
        for (int j = 0; j < 8; j++) {
            auto x =  i % WIDTH;
            auto y = (i / WIDTH) * 8 + j;
            
            auto bit = block >> j;
            dst[y * WIDTH + x] = ((bit & 0b00000001) == 0b00000001) ? 0xff : 0x00;
        }
    }
}

inline size_t MaschineBitmapByteSizeFor(int w, int h) {
    return (w * h) / 3 * 2;
}

inline void ConvertArduboyToMaschine(const uint8_t * src, uint8_t * dst, int w, int h) {
    for (int i = 0; i < w * h; i++) {
        uint8_t srcColor;
        
        {
            auto x = i % w;
            auto y = i / w;
            
            auto srcBlock = src[(y / 8) * w + x];
            auto bitIndex = y % 8;
            auto bitMask  = 1 << bitIndex;
            
            srcColor = (srcBlock & bitMask) >> bitIndex;
        }
        
        {
            uint8_t dstColor = srcColor ? 0x1F : 0x00;
            size_t  dstPixel = (i / 3) * 2;

            switch (i % 3)
            {
                case 0:
                    dst[dstPixel    ]  = dstColor << 3;
                    break;
                    
                case 1:
                    dst[dstPixel    ] |= dstColor >> 2;
                    dst[dstPixel + 1]  = dstColor << 6;
                    break;
                    
                case 2:
                    dst[dstPixel + 1] |= dstColor;
                    break;
            }
        }
    }
}

// - //

void setup();
void loop();

#endif /* MaschineBoi_h */
