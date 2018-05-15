//
//  ArduBoy2.h
//  pacmaschine
//
//  Created by Antonio Malara on 11/05/2018.
//  Copyright © 2018 Antonio Malara. All rights reserved.
//

#ifndef ArduBoy_hpp
#define ArduBoy_hpp

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <algorithm>

#include "Arduboy2Font.h"

#define PROGMEM
#define byte    uint8_t
#define boolean bool

using namespace std;

extern const int       WIDTH  = 255;
extern const int       HEIGHT =  64;

void want_a_frame(const uint8_t * data, int w, int h);

struct Rect {
	int x, y, width, height;
};

struct Point {
	int x, y;
};

class Sprites {
public:
	
	void drawPlusMask(int x, int y, const uint8_t * mask, int frame) {
		
	}
	
};

enum Colors {
	BLACK,
	WHITE,
};

enum Buttons {
	UP_BUTTON,
	DOWN_BUTTON,
	RIGHT_BUTTON,
	LEFT_BUTTON,
	A_BUTTON,
	B_BUTTON,
};

class Arduboy2 {
public:
	
	struct Audio {
		bool enabled;
		Audio() : enabled(false) {}
		void on() {}
		void off() {}
	} audio;

	// -- //

	bool collide(Point point, Rect rect) {
		return ((point.x >= rect.x) && (point.x < rect.x + rect.width) &&
				(point.y >= rect.y) && (point.y < rect.y + rect.height));
	}
	
	bool collide(Rect rect1, Rect rect2) {
		return !(rect2.x                >= rect1.x + rect1.width  ||
				 rect2.x + rect2.width  <= rect1.x                ||
				 rect2.y                >= rect1.y + rect1.height ||
				 rect2.y + rect2.height <= rect1.y);
	}

	// -- //
	
	int framerate = 30;
	
	void setFrameRate(int rate) {
		framerate = rate;
	}
	
	void begin() {
		
	}
	
	void clear() {
		memset(sBuffer, 0, WIDTH * HEIGHT);
	}

	bool nextFrame() {
		usleep(1/framerate * 1000000);
		return true;
	}
	
	void delay() {
		
	}
	
	bool everyXFrames(int frames) {
		return false;
	}
	
	// -- //
	
	uint8_t sBuffer[WIDTH * HEIGHT];
	
	void drawPixel(int16_t x, int16_t y, Colors color) {
		sBuffer[y * WIDTH + x] = color;
	}
	
	void drawCircle(int16_t x0, int16_t y0, uint16_t r, Colors color = WHITE) {
		int16_t f = 1 - r;
		int16_t ddF_x = 1;
		int16_t ddF_y = -2 * r;
		int16_t x = 0;
		int16_t y = r;
		
		drawPixel(x0,     y0 + r, color);
		drawPixel(x0,     y0 - r, color);
		drawPixel(x0 + r, y0,     color);
		drawPixel(x0 - r, y0,     color);
		
		while (x < y)
		{
			if (f >= 0)
			{
				y--;
				ddF_y += 2;
				f += ddF_y;
			}
			
			x++;
			ddF_x += 2;
			f += ddF_x;
			
			drawPixel(x0 + x, y0 + y, color);
			drawPixel(x0 - x, y0 + y, color);
			drawPixel(x0 + x, y0 - y, color);
			drawPixel(x0 - x, y0 - y, color);
			drawPixel(x0 + y, y0 + x, color);
			drawPixel(x0 - y, y0 + x, color);
			drawPixel(x0 + y, y0 - x, color);
			drawPixel(x0 - y, y0 - x, color);
		}
	}
	
	void drawFastVLine (int x,
						int y,
						int h,
						Colors color)
	{
		int end = y + h;
		for (int a = max(0, y); a < min(end, HEIGHT); a++)
		{
			drawPixel(x,a,color);
		}
	}

	void fillCircleHelper(int x0,
						  int y0,
						  int r,
						  int sides,
						  int delta,
						  Colors color)
	{
		int16_t f = 1 - r;
		int16_t ddF_x = 1;
		int16_t ddF_y = -2 * r;
		int16_t x = 0;
		int16_t y = r;
		
		while (x < y)
		{
			if (f >= 0)
			{
				y--;
				ddF_y += 2;
				f += ddF_y;
			}
			
			x++;
			ddF_x += 2;
			f += ddF_x;
			
			if (sides & 0x1) // right side
			{
				drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
				drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
			}
			
			if (sides & 0x2) // left side
			{
				drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
				drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
			}
		}
	}
	
	void fillCircle(int x, int y, int r, Colors color = WHITE) {
		drawFastVLine(x, y - r, 2 * r + 1, color);
		fillCircleHelper(x, y, r, 3, 0, color);
	}
	
	
	// -- //
	
	void drawBitmap(int x,
					int y,
					const uint8_t * bitmap,
					int w,
					int h,
					Colors color = BLACK)
	{
		if (x+w < 0 || x > WIDTH-1 || y+h < 0 || y > HEIGHT-1)
			return;
		
		int yOffset = abs(y) % 8;
		int sRow = y / 8;
		
		if (y < 0) {
			sRow--;
			yOffset = 8 - yOffset;
		}
		
		int rows = h / 8;
		if (h % 8 != 0)
			rows++;
		
		for (int a = 0; a < rows; a++) {
			int bRow = sRow + a;
			
			if (bRow > (HEIGHT / 8) - 1)
				break;
			
			if (bRow > -2) {
				for (int iCol = 0; iCol < w; iCol++) {
					if (iCol + x > (WIDTH -1 ))
						break;
					
					if (iCol + x >= 0) {
						if (bRow >= 0) {
							if (color == WHITE)
								sBuffer[(bRow * WIDTH) + x + iCol] |=   bitmap[(a * w)+iCol] << yOffset;
							else if (color == BLACK)
								sBuffer[(bRow * WIDTH) + x + iCol] &= ~(bitmap[(a * w)+iCol] << yOffset);
							else
								sBuffer[(bRow * WIDTH) + x + iCol] ^=   bitmap[(a * w)+iCol] << yOffset;
						}
						
						if (yOffset && bRow<(HEIGHT/8)-1 && bRow > -2) {
							if (color == WHITE)
								sBuffer[((bRow+1)*WIDTH) + x + iCol] |=   bitmap[(a*w)+iCol]  >> (8 - yOffset);
							else if (color == BLACK)
								sBuffer[((bRow+1)*WIDTH) + x + iCol] &= ~(bitmap[(a*w)+iCol]) >> (8 - yOffset);
							else
								sBuffer[((bRow+1)*WIDTH) + x + iCol] ^=   bitmap[(a*w)+iCol]  >> (8 - yOffset);
						}
					}
				}
			}
		}
	}
	
	void display() {
		want_a_frame(sBuffer, WIDTH, HEIGHT);
	}
	
	// -- //
	
	int cursor_x = 0;
	int cursor_y = 0;
	
	void setCursor(int x, int y) {
		cursor_x = x;
		cursor_y = y;
	}
	
	void drawChar(int x,
				  int y,
				  unsigned char c,
				  Colors color,
				  Colors bg,
				  uint8_t size)
	{
		uint8_t line;
		bool draw_background = bg != color;
		
		const unsigned char* bitmap = font + c * 5;
		
		if ((x >= WIDTH) ||              // Clip right
			(y >= HEIGHT) ||             // Clip bottom
			((x + 5 * size - 1) < 0) ||  // Clip left
			((y + 8 * size - 1) < 0)     // Clip top
			)
		{
			return;
		}
		
		for (uint8_t i = 0; i < 6; i++ )
		{
			line = *bitmap++;
			if (i == 5) {
				line = 0x0;
			}
			
			for (uint8_t j = 0; j < 8; j++)
			{
				Colors draw_color = (line & 0x1) ? color : bg;
				
				if (draw_color || draw_background) {
					for (uint8_t a = 0; a < size; a++ ) {
						for (uint8_t b = 0; b < size; b++ ) {
							drawPixel(x + (i * size) + a, y + (j * size) + b, draw_color);
						}
					}
				}
				
				line >>= 1;
			}
		}
	}

	const int textSize = 1;
	const int textWrap = 0;

	const Colors textColor = WHITE;
	const Colors textBackground = BLACK;
	
	void printCharacter(char c) {
		if (c == '\n')
		{
			cursor_y += textSize * 8;
			cursor_x = 0;
		}
		else if (c == '\r')
		{
			// skip em
		}
		else
		{
			drawChar(cursor_x, cursor_y, c, textColor, textBackground, textSize);
			cursor_x += textSize * 6;
			
			if (textWrap && (cursor_x > (WIDTH - textSize * 6)))
			{
				// calling ourselves recursively for 'newline' is
				// 12 bytes smaller than doing the same math here
				printCharacter('\n');
			}
		}
	}
	
	void print(const char * text) {
		while (*text != 0) {
			printCharacter(*text);
			text++;
		}
	}
	
	void print(int i) {
		char buf[20];
		snprintf(buf, sizeof(buf), "%d", i);
		print(buf);
	}
	
	// -- //
	
	void pollButtons() {
		
	}
	
	bool justPressed(Buttons button) {
		return false;
	}
	
	bool pressed(Buttons button) {
		return false;
	}
	
	// -- //
	
	void initRandomSeed() {
		
	}
	
};

enum Pins {
	PIN_SPEAKER_1,
	PIN_SPEAKER_2,
};

class ArduboyPlaytune {
public:
	ArduboyPlaytune(bool _) {}
	
	void initChannel(Pins pin) {}
	
	void playScore(const uint8_t *) {}
	void stopScore() {}
	
	bool playing() { return false; }
};

int random(int a, int b = 0) {
	return (rand() % (a - b)) + b;
}

void delay(int ms) {
	usleep(ms * 1000);
}


// - //

void createEnemies();
void updateEnemies();
void setTarget();
void checkButtons();
void checkIfScored();
void updatePlayer();
void drawPlayer();

#endif /* ArduBoy_hpp */
