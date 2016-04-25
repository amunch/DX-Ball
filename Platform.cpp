#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "Platform.h"
#include "LTexture.h"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 480;

using namespace std;

extern LTexture gPlatformTexture;

//platform functions
Platform::Platform() {
	//Initialize the platform position to the middle of the screen, slightly above the bottom.
	mPosX = SCREEN_WIDTH/2;	
	mPosY = SCREEN_HEIGHT-30;
	//Initialize the velocity as 0.
	mVelX = 0;
	mVelY = 0;
	lives=3;
	score=0;
	hasGun=false;
	bulletOnScreen=false;
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

	//if space was pressed
	else if (e.type == SDL_KEYDOWN) {
		if(e.key.keysym.sym == SDLK_SPACE && hasGun && !bulletOnScreen) {
			bulletOnScreen=true;
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

//get and set functions for lives
int Platform::getLives() {
	return lives;
}

void Platform::setLives(int l) {
	lives=l;
}
//add the power up
void Platform::addPowerUp(int pu) {
	if(pu==1) {
		hasGun=true;
	}
}
//get and set for score
int Platform::getScore() {
	return score;
}

void Platform::setScore(int s) {
	score=s;
}

//get and set for gun powerup
bool Platform::getHasGun() {
	return hasGun;
}

void Platform::setHasGun(bool s) {
	hasGun=s;
}
//bullet
bool Platform::getBulletOnScreen() {
	return bulletOnScreen;
}

void Platform::setBulletOnScreen(bool s) {
	bulletOnScreen=s;
}

//reset for next life
void Platform::reset() {
	mPosX = SCREEN_WIDTH/2;	
	mPosY = SCREEN_HEIGHT-30;
	mVelX = 0;
	mVelY = 0;
<<<<<<< HEAD
	hasGun=false;
	bulletOnScreen=false;
=======
	cout<<score<<endl;
	cout<<lives<<endl;
>>>>>>> parent of ea55b7c... fixed one thing
}
