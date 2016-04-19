#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "Ball.h"
#include "Platform.h"

#ifndef Box_H
#define Box_H

class Box {
        public:
                //Dimensions of the Box
                static const int Box_WIDTH=19;
                static const int Box_HEIGHT=19;
                //Max total velocity of the Box
                static const int Box_VEL=3;
                //Constructor to initialize variables
                Box(int,int,int);
                //Moves the Box automatically
                int move();
		int hitPlatform(Platform);
                //Shows the platform on the screen
                void render();
		//box if off the screen
		bool offScreen();
		//set position of box
		void setPos(int,int,int);
		//get and set for show box
		void setShow(int);
		int getShow();
        private:
                //X and y position
                int xPos, yPos;
                //Velocity of the Box in x and y directions
                int xVel, yVel;
		//the power up type
		int powerUp;
		// show the box
		int showBox;
};

#endif
