#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "LTexture.h"
#include "Brick.h"

extern LTexture gBrickTexture;

Brick::Brick(int x, int y, int brickType) {
	Box.x = x;
	Box.y = y;

	Box.w = 10;
	Box.h = 5;

	type = brickType;
}

void Brick::render() {
	gBrickTexture.render(Box.x, Box.y);
}

int Brick::getType() {
	return type;
}

SDL_Rect Brick::getBox() {
	return Box;
}
