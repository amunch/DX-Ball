#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "LTexture.h"
#include "Brick.h"

const int TOTAL_BRICKS = 10;

const int BRICK_WIDTH = 30;
const int BRICK_HEIGHT = 10;

extern LTexture gBrickTexture;
extern SDL_Rect BrickClips[TOTAL_BRICKS];

Brick::Brick(int x, int y, int brickType) {
	Box.x = x;
	Box.y = y;

	Box.w = BRICK_WIDTH;
	Box.h = BRICK_HEIGHT;

	type = brickType;
}

void Brick::render() {
	gBrickTexture.render(Box.x, Box.y, &BrickClips[type]);
}

int Brick::getType() {
	return type;
}

void Brick::setType(int new_type) {
	type = new_type;
}

SDL_Rect Brick::getBox() {
	return Box;
}
