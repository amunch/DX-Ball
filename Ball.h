#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "Platform.h"
#include "Brick.h"

extern const int TOTAL_BRICKS;

#ifndef BALL_H
#define BALL_H

class Ball {
        public:
                //Dimensions of the ball
                static const int BALL_WIDTH=10;
                static const int BALL_HEIGHT=10;
                //Max total velocity of the ball
                static const int BALL_VEL=10;
                //Constructor to initialize variables
                Ball();
                //Moves the ball automatically
                bool move(Platform p, Brick* bricks[]);
		//Check to see if the ball has hit any of the bricks.
		bool brickCollision(SDL_Rect ball, SDL_Rect brick);
                //Shows the platform on the screen
                void render();
		//check if off the screen
		bool checkDeath(Platform);
		//get functions
		int getXPos();
		int getYPos();
        private:
                //X and y position
                int xPos, yPos;
                //Velocity of the ball in x and y directions
                int xVel, yVel;
		//SDL_Rect object to determine the edges of the ball.
		//Used for collision purposes.
		SDL_Rect ballBox;
};

#endif
