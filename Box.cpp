#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "Box.h"
#include "Platform.h"
#include "LTexture.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

extern LTexture gBoxTexture;

//Box functions
Box::Box(int x, int y) {
	//Initialize where the Box will be at first.
	xPos = x;
	yPos = y;

	//Initialize the velocity to go diagonally right.
	yVel = 1;
	xVel = 0;
}

int Box::move() {
	if(yPos>SCREEN_HEIGHT) {
		return 0;
	}
	//Move the Box left or right
	xPos += xVel;
	yPos += yVel;
	return 1;
}

int Box::hitPlatform(Platform platform) {
	//in x range
	if ((xPos>=platform.getXPos())&&(xPos<=platform.getXPos()+platform.PLATFORM_WIDTH)) {
		//in y range
		if((yPos>=platform.getYPos()-30)&&(yPos<=platform.getYPos()+platform.PLATFORM_HEIGHT)) {
			return 0;
		}
	}
	return 1;
}

//Render the Box to the reason.
void Box::render() {
	gBoxTexture.render( xPos, yPos );
}