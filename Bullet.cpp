#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "Bullet.h"
#include "Platform.h"
#include "LTexture.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

extern LTexture gBulletTexture;

//Bullet functions
Bullet::Bullet(int x, int y) {
	//Initialize where the Bullet will be at first.
	xPos = x;
	yPos = y;

	//Initialize the velocity to go up.
	yVel = -2;
	xVel = 0;
}

int Bullet::move() {
	if(yPos<0) {
		return 0;
	}
	//Move the Bullet left or right
	xPos += xVel;
	yPos += yVel;
	return 1;
}



//Render the Bullet to the reason.
void Bullet::render() {
	gBulletTexture.render( xPos, yPos );
}

