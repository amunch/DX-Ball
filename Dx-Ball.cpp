//Dx-Ball.cpp
//Andy Munch and Mark Pruitt
//Prints a ball bouncing and platform to the screen.  
//Compile using 'g++ -lSDL -lSDL2_image Dx-Ball.cpp' or use provided Makefile
//Must be run locally on a student machine, do not run remotely (rendering error)

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <sstream>
#include <stdlib.h>

#include "LTexture.h"
#include "Platform.h"
#include "Ball.h"
#include "Brick.h"
#include "Box.h"
#include "LTimer.h"
#include "Bullet.h"

using namespace std;

//Screen dimension global constants
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 480;
//Brick width and height global constants.
const int BRICK_WIDTH = 30;
const int BRICK_HEIGHT = 10;
//Max number of bricks to be rendered.
const int TOTAL_BRICKS = 360;
//Converts color to an integer at a global level.
const int BRICK_NONE = 0;
const int BRICK_BLUE = 1;
const int BRICK_RED = 2;
const int BRICK_GREEN = 3;
const int BRICK_PINK = 4;
//Global FPS declaration.
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

//Starts up SDL and creates window
bool init();
//Loads media
bool loadMedia(Brick* bricks[]);
//Frees media and shuts down SDL
void close(Brick* bricks[]);
//Create an array of Brick objects
bool setBricks(Brick* bricks[],int f);
//The window we'll be rendering to
SDL_Window* gWindow = NULL;
//The window renderer
SDL_Renderer* gRenderer = NULL;
//Check to see if the player has won
bool won(Brick* bricks[]);
//run each level
bool runLevel(Brick* brickset[], Box box, Platform platform, vector<Ball> ballVec, Bullet bullet);
//run the title screen/game over
void titleScreen(string f);
//wait for ball
bool waitForNewBall(SDL_Event e);

//Scene textures for the ball, platform, bricks, and text.
LTexture gPlatformTexture;
LTexture gBallTexture;
LTexture gBrickTexture;
LTexture gBackground;
LTexture gBoxTexture;
LTexture gBulletTexture;
LTexture gScoreText;
LTexture gLivesText;
LTexture gWaitText;
TTF_Font *gFont = NULL;
int score = 0; //to deal with scope
int lives = 3; //to deal with scope

//SDL_Rect used to determine how the sprite sheet will be clipped. 
SDL_Rect BrickClips[TOTAL_BRICKS];

bool init() {
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
		gWindow = SDL_CreateWindow( "Dx-Ball", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				//Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
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
	//If any of the sprites fail to load, print the error and exit.
	if( !gPlatformTexture.loadFromFile( "sprites/basicPlatform.bmp" ) ) {
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	if( !gBallTexture.loadFromFile( "sprites/ball2.bmp" ) ) {
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	if( !gBackground.loadFromFile( "sprites/background.gif" ) ) {
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	if( !gBrickTexture.loadFromFile( "sprites/BrickSheet.bmp" ) ) {
                printf( "Failed to load brick texture!\n" );      
		success = false;
        }
	
	if( !gBulletTexture.loadFromFile( "sprites/bullet.bmp" ) ) {
                printf( "Failed to load bullet texture!\n" );      
		success = false;
        } 

	if(!gBoxTexture.loadFromFile("sprites/box.bmp")) {
		printf("Failed to load box!\n");
		success = false;
	}
	//Initialize font, exit if not initialized properly.
	gFont = TTF_OpenFont( "Fonts/Sansation-Regular.ttf", 28 );
	return success;
}

void close(Brick* bricks[]) {
	//Delete the brick objects.
	for(int i = 0; i < TOTAL_BRICKS; i++) {
		if(bricks[i] == NULL) {	
			delete bricks[i];
			bricks[i] == NULL;
		}
	}
	//Close the font and derefrence the pointer.
	TTF_CloseFont( gFont );
	gFont = NULL;

	//Free loaded images
	gPlatformTexture.free();
	gBallTexture.free();
	gBrickTexture.free();
	gBackground.free();
	gBoxTexture.free();
	gBulletTexture.free();
	gScoreText.free();
	gLivesText.free();
	gWaitText.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

bool setBricks(Brick* bricks[],int currentLvl) {
	bool bricksLoaded = true;
	//Brick placement offsets.
	int x = 0; int y = 0;
	
	//Make sure you load the right map based on the current level.
	string currlvl = static_cast<ostringstream*>( &(ostringstream() <<currentLvl) )->str(); //convert to string
	string filename="maps/map"+currlvl+".map";	
	ifstream map(filename.c_str());
	
	if(map == NULL) {
		cout << "Could not open map" << endl;
		bricksLoaded = false;
	}
	else {
		for(int i = 0; i < TOTAL_BRICKS; i++) {	
			//Instantiate the brick objects.
			int brickType = -1;	
			//Type is taken from the map.
			map >> brickType;
		
			//If the map fails to proveide enough bricks, print the error.
			if(map.fail()) {
				cout << "Error loading map: unexpected EOF" << endl;
				bricksLoaded = false;
				break;
			}
			if(brickType >= 0 && brickType <= 4) {	
				//If the brick type is a valid type, create a new brick object.
				bricks[i] = new Brick(x, y, brickType);
			}
			else {
				//If there are invalid bricks in the map, break and alert the user.
				cout << "Error Loading Map: invalid block type." << endl;
                                bricksLoaded = false;
                                break;
			}
			//Go onto the next brick position.
			x += BRICK_WIDTH;
			if(x >= SCREEN_WIDTH) {
				//If the brick reaches the end of the screen, go to the next row.
				x = 0;
	 			y += BRICK_HEIGHT;
			}
		}
	}
	//At this point, we have created objects for each of the bricks... including their location and type.
	if(bricksLoaded) {
		//Pixel declarations for where on the brick sprite sheet each color brick is located.
		BrickClips[BRICK_BLUE].x = 0;
		BrickClips[BRICK_BLUE].y = 0;
		BrickClips[BRICK_BLUE].w = BRICK_WIDTH;
		BrickClips[BRICK_BLUE].h = BRICK_HEIGHT;
		//Each brick is 30 to the right of the other.
		BrickClips[BRICK_RED].x = 30;
		BrickClips[BRICK_RED].y = 0;
                BrickClips[BRICK_RED].w = BRICK_WIDTH;
                BrickClips[BRICK_RED].h = BRICK_HEIGHT;

		BrickClips[BRICK_GREEN].x = 60;
		BrickClips[BRICK_GREEN].y = 0;
                BrickClips[BRICK_GREEN].w = BRICK_WIDTH;
                BrickClips[BRICK_GREEN].h = BRICK_HEIGHT;

		BrickClips[BRICK_PINK].x = 90;
		BrickClips[BRICK_PINK].y = 0;
                BrickClips[BRICK_PINK].w = BRICK_WIDTH;
                BrickClips[BRICK_PINK].h = BRICK_HEIGHT;	
	}
	//Close the map and return the bricks were properly loaded.
	map.close();
	return bricksLoaded;
}
//all the bricks are gone
bool won(Brick* bricks[]) {
	for(int i = 0; i < TOTAL_BRICKS; i++) { //loop through
		if(bricks[i]->getType() > 0) { //one is not 0
			return false;
		}
	}
	//Return true if all bricks are greater than 0.
	return true;
}

//wait for the user to want new ball
bool waitForNewBall(SDL_Event e) {
	bool quit=false;

	//Display the text when the user died, to press enter for a new ball.
	SDL_Color textColor = { 0, 50, 150 }; //text color
	gWaitText.loadFromRenderedText("Press Enter for New Ball", textColor );
	gWaitText.render(155,300); //Render text.
	SDL_RenderPresent( gRenderer );

	//Wait until the user inputs anything on their decision to quit or continue.
	if(SDL_PollEvent(&e)) {};
	while (!quit) {
		//Clear the event queue if there is anything in it.
		SDL_PumpEvents();
		SDL_FlushEvents(SDL_USEREVENT, SDL_LASTEVENT);
		while( SDL_PollEvent( &e ) != 0 ) {
			//Handle events on queue
			if( e.type == SDL_KEYDOWN ) {
				if(e.key.keysym.sym==SDLK_RETURN) { //pressed enter
						quit = true;
				}	
			} else if( e.type == SDL_QUIT ) { //want to quit game
					return true;
			}
			SDL_PumpEvents();
                	SDL_FlushEvents(SDL_USEREVENT, SDL_LASTEVENT);	
		}	
	}
	return false; //do not want to quit game
}

//Basically the main program for each level
bool runLevel(Brick* brickSet[], Box box, Platform platform, vector<Ball> ballVec, Bullet bullet) {
	//While application is running
	SDL_Event e;

	//Truth values on if the user has won or quit the game.
	bool wonLvl = false;
	bool quit = false;
	
	//Timers for use in limiting the FPS to 60
	//This is used to prevent ball speedup as blocks do not need to be rendered.
	LTimer fpsTimer;
	LTimer capTimer;
	int countedFrames = 0;
	fpsTimer.start();	

	int reset = 0;

	while( !quit ) {
		capTimer.start(); //start timer

		//Run this sequence if there are no more balls on the screen.
		if(ballVec.size() < 1 && !reset) { //if no more balls
			lives--; //remove a life
			if(lives < 1) {  //no lives left
				quit = 1;
			} else { //still have lives left
				//Make sure platform events are clear.			
				reset = 1;
				//Reload the platform and get rid of the gun powerup.
				gPlatformTexture.loadFromFile("sprites/basicPlatform.bmp");
				platform.setHasGun(false); //get rid of gun powerup
			} 
		}

		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 ) {
		//User requests quit
			if( e.type == SDL_QUIT ) {
				quit = true;
			}
			//Handle input for the platform movement
			if(reset) {
				if( e.type == SDL_KEYDOWN ) {				
					if(e.key.keysym.sym==SDLK_RETURN) { //pressed enter						
							reset = 0;
							Ball ball2; //add a new ball
				
							//Add the new ball to the ball vector.
							ballVec.push_back(ball2);
					}	
				}
			}		
			if(platform.handleEvent(e, bullet)) {
				if(!reset) {				
					bullet.setPos(platform.getXPos() + 42, platform.getYPos(), true);
				}			
			}				
			SDL_PumpEvents();
	                SDL_FlushEvents(SDL_USEREVENT, SDL_LASTEVENT);
		}

		//Calculate the average FPS.
		float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
		if(avgFPS > 2000000) {avgFPS = 0;}

		//If the level is won.
		if(won(brickSet)) { //no more bricks so lvl done
			//Set quit and won level to true
			quit = true;
			wonLvl = true;
		}
		
		//Move the objects
		platform.move();
		for(int g=0; g<ballVec.size(); g++) {
			if(ballVec[g].move(platform, brickSet) && box.offScreen()) {
				int powerup = rand() % 6; //random number to get powerup
				if (powerup==1) { //got box!
					//Draw the box if the random number has given you a box.
					int boxX = ballVec[g].getXPos(); 
					int boxY = ballVec[g].getYPos();
					box.setPos(boxX,boxY,1);
				}
				//Increment the score by 5 every box hit.
				score += 5;
			}
		}
		if(box.getShow() && !reset) { //box is on screen
			box.setShow(box.move()); //move downwards
			if(box.hitPlatform(platform)) { //platform hit box
 				box.setPos(700,700,0); //move box off screen
				int powerUp = rand()%3; //what powerup
 				//Powerup that gives you a life.
				if (powerUp==0) {
					lives++;
				} 
				//Powerup that gives you a gun.
				else if (powerUp==1) {
					gPlatformTexture.loadFromFile("sprites/redPUPlatform.bmp");
					platform.setHasGun(true);
				} 
				else { //extra ball
					Ball newBall;
					ballVec.push_back(newBall);
				}					
			} else if (box.offScreen()) { //ball is off screen so dont show
				box.setShow(0);
			}
		}
		
		//handle the bullet
		if(bullet.getShowBullet()) {
			//Move the bullet using the velocities.
			bullet.move();
			if(bullet.hitBrick(brickSet)) { //hit a brick
				bullet.setPos(700,700,false); //move it off screen
			}
		}
		
		//Clear screen
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 ); //black background
		SDL_RenderClear( gRenderer );

		//Render objects
		//gBackground.render(0,0);
		
		platform.render();
		for(int j=0; j<ballVec.size(); j++) { //loop through vector of balls
			if(!ballVec[j].checkDeath(platform)) { //if the ball is still good
				ballVec[j].render(); //render it
			} 
			else { //ball below line
				ballVec.erase(ballVec.begin()+j); //erase it from vector
				//Case where no more balls on screen will be dealt with later
			}
		}
		
		//Render bullet and box if they should be on the screen.
		if(box.getShow()) {  //box on screen
			box.render(); 
		}
		if(bullet.getShowBullet()) {
			bullet.render();
		}
	
		//Render the bricks if they are not of type 0.
		//Will be type 0 if they were hit or never existed.	
		for(int i = 0; i < TOTAL_BRICKS; i++) {
			if(brickSet[i]->getType() != 0) { //if box is there	
				brickSet[i]->render();
			}
		}

		if(reset) {
			SDL_Color textColor = { 0, 50, 150 }; //text color
			gWaitText.loadFromRenderedText("Press Enter for New Ball", textColor );
			gWaitText.render(155,300); //Render text
		}
		
		//render text
		SDL_Color textColor = { 0, 50, 150 }; //text color
		string strScore = static_cast<ostringstream*>( &(ostringstream() <<score) )->str(); //convert to string
		//Render the text for the score.
		gScoreText.loadFromRenderedText("Score: "+strScore, textColor );
		gScoreText.render(0,0); //render score text
		string strLives = static_cast<ostringstream*>( &(ostringstream() <<lives) )->str(); //convert to string
		//Render the text for the lives.
		gLivesText.loadFromRenderedText("Lives: "+strLives,textColor);
		gLivesText.render(SCREEN_WIDTH-125,0);	
	
		//Update screen
		SDL_RenderPresent( gRenderer );
		countedFrames++;

		//If the frame rate gets over the desired 60 FPS, 
		//Delay the loop for as long as needed.
		int frameTicks = capTimer.getTicks();
		if(frameTicks < SCREEN_TICK_PER_FRAME) {
			SDL_Delay(SCREEN_TICK_PER_FRAME - frameTicks);
		}

	}
	return wonLvl;
}		

//show the title screen
void titleScreen(string file) {
	SDL_Event titleEvent; //event handler for title

	bool quitTitle = false; //title loop condition

	LTexture gTitle;
	gTitle.loadFromFile("sprites/"+file+".bmp"); //load picture

	while (!quitTitle) {
		while( SDL_PollEvent( &titleEvent ) != 0 ) {
			//User requests quit
			if( titleEvent.type == SDL_KEYDOWN ) {
				if(titleEvent.key.keysym.sym==SDLK_RETURN) //pressed enter
					quitTitle = true;
				}	
				if( titleEvent.type == SDL_QUIT ) {
				quitTitle = true;
			}
		}
		
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
		SDL_RenderClear( gRenderer ); //clear screen
			
		gTitle.render(0,0); //show title screen
		
		if(file!="titleScreen") { //render score
			SDL_Color textColor = { 0, 50, 150 };
			string strScore = static_cast<ostringstream*>( &(ostringstream() <<score) )->str(); //convert to string
			gScoreText.loadFromRenderedText("Score: "+strScore, textColor );
			gScoreText.render(SCREEN_WIDTH/2-50,300); //render score text
		}
		
		SDL_RenderPresent( gRenderer );
	}
}

//show the level intro screen
void levelIntro(int currentLvl) {
	SDL_Event lvlEvent; //event handler for title

	bool quitLvl = false; //title loop condition

	LTexture gLvl;
	string currlvl = static_cast<ostringstream*>( &(ostringstream() <<currentLvl) )->str(); //convert to string
	gLvl.loadFromFile("sprites/levelintro"+currlvl+".bmp"); //load picture

	while (!quitLvl) {
		while( SDL_PollEvent( &lvlEvent ) != 0 ) {
			//User requests quit
			if( lvlEvent.type == SDL_KEYDOWN ) {
				if(lvlEvent.key.keysym.sym==SDLK_RETURN) //pressed enter
					quitLvl = true;
				}	
				if( lvlEvent.type == SDL_QUIT ) {
				quitLvl = true;
			}
		}
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
		SDL_RenderClear( gRenderer ); //clear screen

		gLvl.render(0,0); //show title screen

		SDL_RenderPresent( gRenderer );
	}
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
			int currentLevel=1;
			int maxLevel=5;
			bool wonGame=false; bool loseGame=false;
			titleScreen("titleScreen"); //run the title screen

			srand(time(NULL)); //Re-seed the random number generator.

			//Create objects of each class required to play.
			Platform platform;
			Ball ball;
			Bullet bullet;
			vector<Ball> ballVec;
			ballVec.push_back(ball);
			//Instantiate the box off screen.
			Box box(700,700,0);
		
			while(!wonGame && !loseGame) {

				levelIntro(currentLevel);

				//The platform that will be moving left and right on the screen
				platform.reset(); //reset the platform
				gPlatformTexture.loadFromFile("sprites/basicPlatform.bmp");

				//the ball that will be bouncing around
				Ball newball; 
				ballVec.clear(); //clear the vector and add one new ball
				ballVec.push_back(newball);
		
				//Set the box and bullet off the screen and not rendering.
				box.setPos(700, 700, 0);
				bullet.setPos(700, 700, false);	

				setBricks(brickSet,currentLevel); //load new level

				if(runLevel(brickSet,box,platform,ballVec,bullet)) { //level over
					score=score+100*currentLevel; //add to the score
					currentLevel++;//next level!

					if(currentLevel > maxLevel) { //finished all levels
						wonGame=true; // won game
					}

				} 
				else { //lost the level
					loseGame=true;
				}
			}
			
			if(wonGame) { //won the game
				titleScreen("winscreen"); //reuse title screen to show win
			} 
			else { //lost the game
				titleScreen("losescreen"); //reuse title screen to show lose
			}	
		}
		//Free resources and close SDL
		close(brickSet);
	}
	return 0;
}
