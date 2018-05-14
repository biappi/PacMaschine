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

#define PROGMEM
#define byte    uint8_t
#define boolean bool

#define WIDTH  255
#define HEIGHT  64

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

	bool collide(Point lhs, Rect rhs) {
		return false;
	}
	
	bool collide(Rect  lhs, Rect rhs) {
		return false;
	}

	// -- //
	
	void setFrameRate(int framerate) {
		
	}
	
	void begin() {
		
	}
	
	void clear() {
		
	}

	bool nextFrame() {
		return false;
	}
	
	void delay() {
		
	}
	
	bool everyXFrames(int frames) {
		return false;
	}
	
	// -- //
	
	void drawCircle(int x, int y, int size) {
		
	}
	
	void fillCircle(int x, int y, int size) {
		
	}
	
	
	// -- //
	
	void drawBitmap(uint8_t a,
					uint8_t b,
					const uint8_t * data,
					int c,
					int d,
					Colors color = BLACK)
	{
		
	}
	
	void display() {
		
	}
	
	// -- //
	
	void setCursor(int, int) {
		
	}
	
	void print(const char *) {
		
	}

	void print(int) {
		
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

int random(int a, int b) {
	return 0;
}

void delay(int ms) {
	
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
