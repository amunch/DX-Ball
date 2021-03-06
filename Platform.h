#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "Bullet.h"

#ifndef PLATFORM_H
#define PLATFORM_H

//The platform that will move left and right on the screen
class Platform {
	
	public:
		//The dimensions of the platform
		static const int PLATFORM_WIDTH = 84;
		static const int PLATFORM_HEIGHT = 11;
		//Maximum x axis velocity of the platform
		static const int PLATFORM_VEL = 10;
		//Initializes the variables
		Platform();
		//Takes key presses and adjusts the platform's velocity
		bool handleEvent( SDL_Event& e ,Bullet b);
		//Moves the platform position
		void move();
		//Shows the platform on the screen
		void render();
		//Get functions
		int getXPos();
		int getYPos();
		//get and set for lives
		int getLives();
		void setLives(int);
		//reset for next life
		void reset();
		//get and set for score
		int getScore();
		void setScore(int);
		//get and set for gun powerup
		void setHasGun(bool);
		bool getHasGun();

	private:
		//The X and Y position of the platform
		int mPosX, mPosY;
		//The velocity of the platform
		int mVelX, mVelY;
		//lives for the player
		int lives;
		int score; //score for the player
		//variables for gun powerup
		bool hasGun;
};

#endif
