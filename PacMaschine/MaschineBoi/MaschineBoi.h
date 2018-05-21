//
//  MaschineBoi.h
//  pacmaschine
//
//  Created by Antonio Malara on 16/05/2018.
//  Copyright © 2018 Antonio Malara. All rights reserved.
//

#ifndef MaschineBoi_h
#define MaschineBoi_h

static const int WIDTH  = 255;
static const int HEIGHT =  64;

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

class ButtonsState;

extern ButtonsState globalButtonsState;
extern ButtonsState globalPreviousButtonState;
extern double       globalFramerate;

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

void setup();
void loop();

void want_a_frame(const uint8_t * data, int w, int h);

#endif /* MaschineBoi_h */
