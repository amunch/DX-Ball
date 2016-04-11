#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "Ball.h"
#include "Platform.h"
#include "LTexture.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

extern LTexture gBallTexture;

//Ball functions
Ball::Ball() {
	//Initialize where the ball will be at first.
	xPos = SCREEN_WIDTH/2;
	yPos = SCREEN_HEIGHT-100;

	//Initialize the velocity to go diagonally right.
	yVel = 1;
	xVel = 1;
}

void Ball::move(Platform platform) {
	//Move the ball left or right
	xPos += xVel;
	yPos += yVel;

	//If the ball went too far to the left or right
	if( ( xPos < 0 ) || ( xPos + BALL_WIDTH > SCREEN_WIDTH ) ) {
		//Move back
       		xPos -= xVel;
		xVel = -xVel; //switch directions
    	}
    	//if the ball went too far up or down
    	if ((yPos <0) || (yPos + BALL_HEIGHT > SCREEN_HEIGHT )) {
		//Move back in other direction.
		yPos -= yVel;
		yVel = -yVel;
    	}
    	//hit the platform
    	//in x range
    	if ((xPos>=platform.getXPos())&&(xPos<=platform.getXPos()+platform.PLATFORM_WIDTH)) {
		//in y range
		if((yPos>=platform.getYPos()-20)&&(yPos<=platform.getYPos()+platform.PLATFORM_HEIGHT)) {
             	//get middle of ball and platform
	    		int midBall = xPos + (BALL_WIDTH/2);
	     		int midPlat = platform.getXPos() + (platform.PLATFORM_WIDTH/2);
	     		if(xVel>0) { //ball coming in from the left
				if(midBall<midPlat) {
					xPos -=xVel;
					yPos -=yVel;
					xVel=-xVel;
					yVel=-yVel;
		     		}
				else {
					yPos -=yVel;
					xPos -=xVel;
					yVel=-yVel;
		     		}
	     		} 
			else { //ball coming in from the right
		     		if(midBall>=midPlat) {
					xPos -=xVel;
					yPos -=yVel;
					xVel=-xVel;
					yVel=-yVel;
			     	} 
				else {
					yPos -=yVel;
					xPos -=xVel;
					yVel=-yVel;
		     		}
			}
		}
    	} 
}

//Render the ball to the reason.
void Ball::render() {
	gBallTexture.render( xPos, yPos );
}

