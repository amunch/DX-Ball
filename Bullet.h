#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "Brick.h"

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
		int move();
		//Shows the BULLET on the screen
		void render();
		//hit the brick
		bool hitBrick(Brick* brick[]);
		//collision with bricks
		bool brickCollision(SDL_Rect brick);
		//set the position
		void setPos(int,int,bool);
		bool getShowBullet();
		//off the screen
		bool offScreen();
	private:
		//The X and Y position of the BULLET
		int xPos, yPos;
		//The velocity of the BULLET
		int xVel, yVel;
		//show bullet
		bool showBullet;
};

#endif
