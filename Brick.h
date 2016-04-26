#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>

#ifndef BRICK_H
#define BRICK_H

class Brick {
	public:
		//Constructor for position and type.
		Brick(int x, int y, int brickType);
		//Function to render the brick to the screen.
		void render();
		//Retrieve the type of the block.
		int getType();
		//Set the type of the block
		void setType(int new_type);
		//Retrieve the SDL_Rect object for collision detection.
		SDL_Rect getBox();
	private:
		//Used for collision detection.
		SDL_Rect Box;
		//The type of the box to determine color and health.
		int type;
};

#endif
