#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "Bullet.h"
#include "Platform.h"
#include "LTexture.h"
#include "Brick.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

extern const int TOTAL_BRICKS;

extern LTexture gBulletTexture;

//Bullet functions
Bullet::Bullet() {
	//Initialize where the Bullet will be at first.
	xPos = 700;
	yPos = 700;

	//Initialize the velocity to go up.
	yVel = -3;
	xVel = 0;
	showBullet=false;
}

int Bullet::move() {
	if(yPos+15<0) {
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
	return showBullet;
}

//is the bullet off the screen
bool Bullet::offScreen() {
	if(yPos + 15 < 0 || yPos==700) {
		return true;
	}
	return false;
}

//hit a brick
bool Bullet::hitBrick(Brick *bricks[]) {
	bool hit = false;
	//See if the Bullet has hit any of the bricks.	
	for(int i = 0; i < TOTAL_BRICKS; i++) {
		if(bricks[i]->getType() > 0) {
			if(brickCollision(bricks[i]->getBox())) {		
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

	if(xPos > leftBrick && xPos <= rightBrick && yPos > topBrick && yPos < bottomBrick) {
		return true;
	}
	return false;
}

//Render the Bullet to the reason.
void Bullet::render() {
	gBulletTexture.render( xPos, yPos );
}

