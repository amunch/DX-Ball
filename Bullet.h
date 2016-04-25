#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#ifndef BULLET_H
#define BULLET_H

//The BULLET that will move up from the platform
class Bullet {
	
	public:
		//The dimensions of the BULLET
		static const int BULLET_WIDTH = 3;
		static const int BULLET_HEIGHT = 3;
		//Maximum x axis velocity of the BULLET
		//Initializes the variables
		Bullet();
		//Moves the BULLET position
		void move();
		//Shows the BULLET on the screen
		void render();
		//hit the brick
	private:
		//The X and Y position of the BULLET
		int mPosX, mPosY;
		//The velocity of the BULLET
		int mVelX, mVelY;
};

#endif
