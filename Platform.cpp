#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "Platform.h"
#include "LTexture.h"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 480;

extern LTexture gPlatformTexture;

//platform functions
Platform::Platform() {
	//Initialize the platform position to the middle of the screen, slightly above the bottom.
	mPosX = SCREEN_WIDTH/2;	
	mPosY = SCREEN_HEIGHT-30;
	//Initialize the velocity as 0.
	mVelX = 0;
	mVelY = 0;
}

void Platform::handleEvent( SDL_Event& e ) {
	//If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ) {
        	//Adjust the velocity
        	switch( e.key.keysym.sym ) {
        		case SDLK_LEFT: mVelX -= PLATFORM_VEL; break;
        		case SDLK_RIGHT: mVelX += PLATFORM_VEL; break;
        	}
    	}
	//If a key was released
	else if( e.type == SDL_KEYUP && e.key.repeat == 0 ) {
        	//Adjust the velocity of the platform
        	switch( e.key.keysym.sym ) {
        		case SDLK_LEFT: mVelX += PLATFORM_VEL; break;
        		case SDLK_RIGHT: mVelX -= PLATFORM_VEL; break;
      		}
    	}
}

void Platform::move() {
	//Move the platform left or right along its axis.
	mPosX += mVelX;
	//If the platform went too far to the left or right
	if( ( mPosX < 0 ) || ( mPosX + PLATFORM_WIDTH > SCREEN_WIDTH ) ) {
        	//Move back
        	mPosX -= mVelX;
  	}
}

void Platform::render() {
	//Render the platform to the screen.
	gPlatformTexture.render( mPosX, mPosY );
}

//Get functions for the position of the platform.
int Platform::getXPos() {
	return mPosX;
}

int Platform::getYPos() {
	return mPosY;
} 

//add the power up
void Platform::addPowerUp(int) {

}
