#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

#include "Ball.h"
#include "Platform.h"
#include "Brick.h"
#include "LTexture.h"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 480;

extern const int TOTAL_BRICKS;

extern LTexture gBallTexture;

//Ball functions
Ball::Ball() {
	//Initialize where the ball will be at first.
	xPos = SCREEN_WIDTH/2;
	yPos = SCREEN_HEIGHT-100;

	//Initialize the ball surrounding box to check for collisions.
	ballBox.x = xPos;
	ballBox.y = yPos;
	ballBox.w = BALL_WIDTH;
	ballBox.h = BALL_HEIGHT;

	//Initialize the velocity to go diagonally right.
	yVel = 1;
	xVel = 1;
}

void Ball::move(Platform platform, Brick* bricks[]) {
	//Move the ball left or right
	xPos += xVel;
	yPos += yVel;

	ballBox.x = xPos;
        ballBox.y = yPos;

	cout << "x:" << ballBox.x << endl;
	cout << "y:" << ballBox.y << endl;

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
		if((yPos>=platform.getYPos()-10)&&(yPos<=platform.getYPos()+platform.PLATFORM_HEIGHT)) {
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
	//See if the Ball has hit any of the bricks.	
	for(int i = 0; i < TOTAL_BRICKS; i++) {
		if(bricks[i]->getType() > 0) {
			if(brickCollision(ballBox, bricks[i]->getBox())) {
				yPos -= yVel;
                		yVel = -yVel;			
				bricks[i]->setType(0);
			}
		}
	}
	ballBox.x = xPos;
        ballBox.y = yPos;
}

bool Ball::brickCollision(SDL_Rect ball, SDL_Rect brick) {
	int leftBall, leftBrick;
	int rightBall, rightBrick;
	int topBall, topBrick;
	int bottomBall, bottomBrick;
	//Calculate the sides of the ball.
	leftBall = ball.x;
	rightBall = ball.x + ball.w;
	topBall = ball.y;
	bottomBall = ball.y + ball.h;
	//Calculate the sides of the brick.
	leftBrick = brick.x;
	rightBrick = brick.x + brick.w;
	topBrick = brick.y;
	bottomBrick = brick.y + brick.h;

	cout << brick.w << endl;
	cout << brick.h << endl;

	//If any of the sides of the ball are outside the sides of the brick, there is no collision.
	if(topBall <= bottomBrick && rightBall >= leftBrick && leftBall <= rightBrick) {
		return true;
	}
	return false;	
}

//Render the ball to the reason.
void Ball::render() {
	gBallTexture.render( xPos, yPos );
}

