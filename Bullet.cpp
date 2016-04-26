#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "Bullet.h"
#include "Platform.h"
#include "LTexture.h"
#include "Brick.h"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 480;
//External objects from main.
extern const int TOTAL_BRICKS;

extern LTexture gBulletTexture;

//Bullet functions
Bullet::Bullet() {
	//Initialize where the Bullet will be at first.
	xPos = 700;
	yPos = 700;

	//Initialize the velocity to go up.
	yVel = -5;
	xVel = 0;
	showBullet=false;
}

int Bullet::move() {
	//Kill the bullet if it has gone off the screen.
	if(yPos + 15 < 0) {
		return 0;
	}
	//Move the Bullet left or right
	xPos += xVel;
	yPos += yVel;
	return 1;
}
//set position of bullet
void Bullet::setPos(int x,int y,bool b) {
	xPos=x;
	yPos=y;
	showBullet=b;
}
	
bool Bullet::getShowBullet() {
	//return if the bullet is to be shown.
	return showBullet;
}

//is the bullet off the screen
bool Bullet::offScreen() {
	//Return if the bullet is off the screen (do not redner).
	if(yPos + 15 < 0 || yPos == 700) {
		return true;
	}
	return false;
}

//hit a brick
bool Bullet::hitBrick(Brick *bricks[]) {
	bool hit = false;
	//See if the Bullet has hit any of the bricks.	
	for(int i = 0; i < TOTAL_BRICKS; i++) {
		//If type is 0, the brick is 'dead'.
		if(bricks[i]->getType() > 0) {
			if(brickCollision(bricks[i]->getBox())) {		
				//Decrement the brick type if it did get hit.
				bricks[i]->setType(bricks[i]->getType()-1);
				hit =true;
			}
		}
	}
	return hit;
}

bool Bullet::brickCollision(SDL_Rect brick) {
	int leftBrick;
	int rightBrick;
	int topBrick;
	int bottomBrick;
	//Calculate the sides of the brick.
	leftBrick = brick.x;
	rightBrick = brick.x + brick.w;
	topBrick = brick.y;
	bottomBrick = brick.y + brick.h;
	//If bullet is within a brick, return that it has hit.
	if(xPos > leftBrick && xPos <= rightBrick && yPos > topBrick && yPos < bottomBrick) {
		return true;
	}
	return false;
}

//Render the Bullet to the reason.
void Bullet::render() {
	//Use the LTexture render function at the x and y position.
	gBulletTexture.render( xPos, yPos );
}

