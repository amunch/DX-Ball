#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>

#ifndef BRICK_H
#define BRICK_H

class Brick {
	public:
		Brick(int x, int y, int brickType);
		void render();
		int getType();
		SDL_Rect getBox();
	
	private:
		
		SDL_Rect Box;
		int type;
};

#endif
