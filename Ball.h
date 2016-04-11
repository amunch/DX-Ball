#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "Platform.h"

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
                void move(Platform p);

                //Shows the platform on the screen
                void render();
        private:
                //X and y position
                int xPos, yPos;
                //Velocity of the ball in x and y directions
                int xVel, yVel;
};

#endif
