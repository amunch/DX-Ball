#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "LTexture.h"
#include "Brick.h"

const int TOTAL_BRICKS = 10;

const int BRICK_WIDTH = 30;
const int BRICK_HEIGHT = 10;
//External and constant global variables from main.
extern LTexture gBrickTexture;
extern SDL_Rect BrickClips[TOTAL_BRICKS];

Brick::Brick(int x, int y, int brickType) {
	//Constructor to intialize the SDL_Rect object and the type.
	//Set the x and y positions of the blocks.
	Box.x = x;
	Box.y = y;
	//Set the width and height of the SDL_Rect.
	//Makes it eas for collision deteciton.
	Box.w = BRICK_WIDTH;
	Box.h = BRICK_HEIGHT;
	//Set the type of the brick.
	type = brickType;
}

void Brick::render() {
	//Render using the LTexture object.
	gBrickTexture.render(Box.x, Box.y, &BrickClips[type]);
}

int Brick::getType() {
	return type;
}
//Get and set functions to determine the behavior and color of different type blocks.
void Brick::setType(int new_type) {
	type = new_type;
}

SDL_Rect Brick::getBox() {
	//Return the SDL_Rect object that defines the block's bounds.
	return Box;
}
