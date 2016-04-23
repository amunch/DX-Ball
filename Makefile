all: ballBounce

ballBounce: ballBounce.o LTexture.o Platform.o Ball.o Brick.o Box.o
	g++ -g -lSDL2 -lSDL2_image Ball.o Platform.o ballBounce.o LTexture.o Box.o Brick.o -o ballBounce

ballBounce.o: ballBounce.cpp
	g++ -lSDL2 -lSDL2_image -c ballBounce.cpp 
LTexture.o: LTexture.cpp
	g++ -lSDL2 -lSDL2_image -c LTexture.cpp
Platform.o: Platform.cpp
	g++ -lSDL2 -lSDL2_image -c Platform.cpp
Ball.o: Ball.cpp
	g++ -lSDL2 -lSDL2_image -c Ball.cpp
Box.o: Box.cpp
	g++ -lSDL2 -lSDL2_image -c Box.cpp
Brick.o: Brick.cpp
	g++ -lSDL2 -lSDL2_image -c Brick.cpp

clean: 
	rm ballBounce *.o
