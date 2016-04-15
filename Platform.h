#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

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
		void handleEvent( SDL_Event& e );
		//Moves the platform position
		void move();
		//Shows the platform on the screen
		void render();
		//add power up
		void addPowerUp(int);
		//Get functions
		int getXPos();
		int getYPos();

	private:
		//The X and Y position of the platform
		int mPosX, mPosY;
		//The velocity of the platform
		int mVelX, mVelY;
};

#endif
