#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "Box.h"
#include "Platform.h"
#include "LTexture.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BOX_WIDTH = 19;

extern LTexture gBoxTexture;

//Box functions
Box::Box(int x, int y, int s) {
	//Initialize where the Box will be at first.
	xPos = x;
	yPos = y;

	//Initialize the velocity to go diagonally right.
	yVel = 1;
	xVel = 0;

	showBox=s;
}

int Box::move() {
	if(yPos > SCREEN_HEIGHT) {
		return 0;
	}
	//Move the Box left or right
	xPos += xVel; 
	yPos += yVel; //As stated in initializer, should just speed up downward.
	return 1;
}

int Box::hitPlatform(Platform platform) {
	//in x range
	if ((xPos >= platform.getXPos()) && (xPos <= platform.getXPos() + platform.PLATFORM_WIDTH)) {
		//in y range
		if((yPos >= platform.getYPos() - 19) && (yPos <= platform.getYPos() + platform.PLATFORM_HEIGHT)) {
			return 1;
		}
	}
	return 0;
}

//reset position
void Box::setPos(int x,int y, int s) {
	//so it doesn't appear off screen
	if (x == 700) {}
	else {
		if(x < BOX_WIDTH) { //Makes sure the box does not appear offscreen
			//Used if block is hit near the edge on left.
			x = BOX_WIDTH;
		} else if(x > SCREEN_WIDTH - 19) {
			//Used if block is hit near the right edge.
			x = SCREEN_WIDTH - 19;
		}
		if(y < 19) { //Likewise with y, make it appear below the top of the screen.
			y = 19;
		} else if(y > SCREEN_HEIGHT - 19) {
			//If box starts at bottom.	
			//Likely will not happen but used for consistency.
			y = SCREEN_HEIGHT - 19;
		}
	}
	//Update position.
	xPos = x; yPos = y;
	showBox = s;
}

//check if off screen
bool Box::offScreen() {
	if(yPos > SCREEN_HEIGHT) { //Determines if the box should be rendered if 'off screen'
		return true;
	}
	return false;
}

//Render the Box to the screen at the x and y position.
void Box::render() {
	gBoxTexture.render( xPos, yPos );
}

//set function for show Box
void Box::setShow(int s) {
	showBox = s;
}

//See if the box should be shown.
int Box::getShow() {
	return showBox;
}
