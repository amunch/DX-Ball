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

//Screen dimension global constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Texture wrapper class
class LTexture {

	public:
		//Initializes variables
		LTexture();
		//Deallocates memory
		~LTexture();
		//Loads image at specified path
		bool loadFromFile( std::string path );
		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif
		//Deallocates texture
		void free();
		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );
		//Set blending
		void setBlendMode( SDL_BlendMode blending );
		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
		//Gets image dimensions
		int getWidth();
		int getHeight();
	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
		//Image dimensions
		int mWidth;
		int mHeight;
};

//The application time based timer
class LTimer {

	public:
		//Initializes variables
		LTimer();
		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();
		//Gets the timer's time
		Uint32 getTicks();
		//Checks the status of the timer
		bool isStarted();
		bool isPaused();

	private:
		//The clock time when the timer started
		Uint32 mStartTicks;
		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;
		//The timer status
		bool mPaused;
		bool mStarted;
};

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

LTexture::LTexture() {
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture() {
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path ) {
	//Get rid of preexisting texture
	free();
	//The final texture
	SDL_Texture* newTexture = NULL;
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL ) {
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else {
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL ) {
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else {
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}
	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor ) {
	//Get rid of preexisting texture
	free();
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL ) {
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL ) {
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else {
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else {
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free() {
	//Free texture if it exists
	if( mTexture != NULL ) {
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue ) {
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending ) {
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha ) {
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip ) {
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL ) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth() {
	return mWidth;
}

int LTexture::getHeight() {
	return mHeight;
}

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
