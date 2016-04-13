//ballBounce.cpp
//Andy Munch and Mark Pruitt
//Prints a ball bouncing and platform to the screen.  
//Compile using 'g++ -lSDL -lSDL2_image ballBounce.cpp' or use provided Makefile
//Must be run locally on a student machine, do not run remotely (rendering error)

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "LTexture.h"
#include "Platform.h"
#include "Ball.h"
#include "Brick.h"

using namespace std;

//Screen dimension global constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int BRICK_WIDTH = 10;
const int BRICK_HEIGHT = 5;

const int TOTAL_BRICKS = 10;

//Starts up SDL and creates window
bool init();
//Loads media
bool loadMedia(Brick* bricks[]);
//Frees media and shuts down SDL
void close(Brick* bricks[]);
//Create an array of Brick objects
bool setBricks(Brick* bricks[]);
//The window we'll be rendering to
SDL_Window* gWindow = NULL;
//The window renderer
SDL_Renderer* gRenderer = NULL;
//Scene textures for the ball and the platform
LTexture gPlatformTexture;
LTexture gBallTexture;
LTexture gBrickTexture;

bool init() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else {
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		//Create window
		gWindow = SDL_CreateWindow( "Bouncing Ball", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ) {
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else {
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL ) {
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else {
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}
	return success;
}


bool loadMedia(Brick* bricks[]) {
	//Loading success flag
	bool success = true;

	//Load object texture
	if( !gPlatformTexture.loadFromFile( "platform2.bmp" ) ) {
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	if( !gBallTexture.loadFromFile( "ball2.bmp" ) ) {
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	if( !gBrickTexture.loadFromFile( "platform.bmp" ) ) {
                printf( "Failed to load brick texture!\n" );
                success = false;
        }
	
	if(!setBricks(bricks)) {
		printf("Failed to set the Bricks!\n");
		success = false;
	}
	
	return success;
}

void close(Brick* bricks[]) {
	for(int i = 0; i < TOTAL_BRICKS; i++) {
		if(bricks[i] == NULL) {	
			delete bricks[i];
			bricks[i] == NULL;
		}
	}

	//Free loaded images
	gPlatformTexture.free();
	gBallTexture.free();
	gBrickTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool setBricks(Brick* bricks[]) {
	bool bricksLoaded = true;
	//Brick placement offsets.
	int x = 0; int y = 0;
	
	ifstream map("Bricks.map");
	
	if(map == NULL) {
		cout << "Could not open map" << endl;
		bricksLoaded = false;
	}
	else {
		for(int i = 0; i < TOTAL_BRICKS; i++) {	
			int brickType = -1;	
			map >> brickType;
		
			if(map.fail()) {
				cout << "Error loading map: unexpected EOF" << endl;
				bricksLoaded = false;
				break;
			}
		
			if(brickType >= 0) {	
				bricks[i] = new Brick(x, y, brickType);
			}
			else {
				cout << "Error Loading Map: invalid block type." << endl;
                                bricksLoaded = false;
                                break;
			}

			x += BRICK_WIDTH;
			y += BRICK_HEIGHT;
	
		}
	}
	//At this point, we have created objects for each of the bricks... including their location and type.
	map.close();
	return bricksLoaded;
}
			

int main( int argc, char* args[] ) {
	//Start up SDL and create window
	if( !init() ) {
		printf( "Failed to initialize!\n" );
	}
	else {
		Brick* brickSet[TOTAL_BRICKS];
		
		//Load media
		if( !loadMedia(brickSet) ) {
			printf( "Failed to load media!\n" );
		}
		else {	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The platform that will be moving left and right on the screen
			Platform platform;

			//the ball that will be bouncing around
			Ball ball;

			//While application is running
			while( !quit ) {
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 ) {
					//User requests quit
					if( e.type == SDL_QUIT ) {
						quit = true;
					}
					//Handle input for the platform movement
					platform.handleEvent( e );
				}
				//Move the objects
				platform.move();
				ball.move(platform);

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render objects
				platform.render();
				ball.render();

				for(int i = 0; i < TOTAL_BRICKS; i++) {
					brickSet[i]->render();
				}

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
		//Free resources and close SDL
		close(brickSet);
	}
	return 0;
}
