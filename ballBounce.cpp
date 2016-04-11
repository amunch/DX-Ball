//ballBounce.cpp
//Andy Munch and Mark Pruitt
//Lab 8
//Prints a ball bouncing and platform to the screen.  
//Compile using 'g++ -lSDL -lSDL2_image ballBounce.cpp' or use provided Makefile
//Must be run locally on a student machine, do not run remotely (rendering error)

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "LTexture.h"

//Screen dimension global constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The platform that will move left and right on the screen
class Platform {
	
	public:
		//The dimensions of the platform
		static const int PLATFORM_WIDTH = 100;
		static const int PLATFORM_HEIGHT = 20;
		//Maximum x axis velocity of the platform
		static const int PLATFORM_VEL = 10;
		//Initializes the variables
		Platform();
		//Takes key presses and adjusts the platform's velocity
		void handleEvent( SDL_Event& e );
		//Moves the platform position
		void move();
		//Shows the platform on the screen
		void render();
		//Get functions
		int getXPos();
		int getYPos();

	private:
		//The X and Y position of the platform
		int mPosX, mPosY;
		//The velocity of the platform
		int mVelX, mVelY;
};

class Ball {
	public:
		//Dimensions of the ball
		static const int BALL_WIDTH=20;
		static const int BALL_HEIGHT=20;
		//Max total velocity of the ball
		static const int BALL_VEL=10;
		//Constructor to initialize variables
		Ball();
		//Moves the ball automatically
		void move(Platform p);

		//Shows the platform on the screen
		void render();
	private:
		//X and y position
		int xPos, yPos;
		//Velocity of the ball in x and y directions
		int xVel, yVel;
};

//Starts up SDL and creates window
bool init();
//Loads media
bool loadMedia();
//Frees media and shuts down SDL
void close();
//The window we'll be rendering to
SDL_Window* gWindow = NULL;
//The window renderer
SDL_Renderer* gRenderer = NULL;
//Scene textures for the ball and the platform
LTexture gPlatformTexture;
LTexture gBallTexture;

//platform functions
Platform::Platform() {
	//Initialize the platform position to the middle of the screen, slightly above the bottom.
	mPosX = SCREEN_WIDTH/2;	
	mPosY = SCREEN_HEIGHT-30;
	//Initialize the velocity as 0.
	mVelX = 0;
	mVelY = 0;
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


//Ball functions
Ball::Ball() {
	//Initialize where the ball will be at first.
	xPos = SCREEN_WIDTH/2;
	yPos = SCREEN_HEIGHT-100;

	//Initialize the velocity to go diagonally right.
	yVel = 1;
	xVel = 1;
}

void Ball::move(Platform platform) {
	//Move the ball left or right
	xPos += xVel;
	yPos += yVel;

	//If the ball went too far to the left or right
	if( ( xPos < 0 ) || ( xPos + BALL_WIDTH > SCREEN_WIDTH ) ) {
		//Move back
       		xPos -= xVel;
		xVel = -xVel; //switch directions
    	}
    	//if the ball went too far up or down
    	if ((yPos <0) || (yPos + BALL_HEIGHT > SCREEN_HEIGHT )) {
		//Move back in other direction.
		yPos -= yVel;
		yVel = -yVel;
    	}
    	//hit the platform
    	//in x range
    	if ((xPos>=platform.getXPos())&&(xPos<=platform.getXPos()+platform.PLATFORM_WIDTH)) {
		//in y range
		if((yPos>=platform.getYPos()-20)&&(yPos<=platform.getYPos()+platform.PLATFORM_HEIGHT)) {
             	//get middle of ball and platform
	    		int midBall = xPos + (BALL_WIDTH/2);
	     		int midPlat = platform.getXPos() + (platform.PLATFORM_WIDTH/2);
	     		if(xVel>0) { //ball coming in from the left
				if(midBall<midPlat) {
					xPos -=xVel;
					yPos -=yVel;
					xVel=-xVel;
					yVel=-yVel;
		     		}
				else {
					yPos -=yVel;
					xPos -=xVel;
					yVel=-yVel;
		     		}
	     		} 
			else { //ball coming in from the right
		     		if(midBall>=midPlat) {
					xPos -=xVel;
					yPos -=yVel;
					xVel=-xVel;
					yVel=-yVel;
			     	} 
				else {
					yPos -=yVel;
					xPos -=xVel;
					yVel=-yVel;
		     		}
			}
		}
    	} 
}

//Render the ball to the reason.
void Ball::render() {
	gBallTexture.render( xPos, yPos );
}

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


bool loadMedia() {
	//Loading success flag
	bool success = true;

	//Load object texture
	if( !gPlatformTexture.loadFromFile( "platform.bmp" ) ) {
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	if( !gBallTexture.loadFromFile( "ball.bmp" ) ) {
		printf( "Failed to load dot texture!\n" );
		success = false;
	}
	return success;
}

void close() {
	//Free loaded images
	gPlatformTexture.free();
	gBallTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] ) {
	//Start up SDL and create window
	if( !init() ) {
		printf( "Failed to initialize!\n" );
	}
	else {
		//Load media
		if( !loadMedia() ) {
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

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
