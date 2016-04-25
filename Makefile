all: dxBall

dxBall: dxBall.o LTexture.o Platform.o Ball.o Brick.o Box.o LTimer.o
	g++ -g -lSDL2 -lSDL2_ttf -lSDL2_image Ball.o Platform.o dxBall.o LTexture.o LTimer.o Box.o Brick.o -o dxBall

dxBall.o: dxBall.cpp
	g++ -lSDL2 -lSDL2_image -c dxBall.cpp 
LTexture.o: LTexture.cpp
	g++ -lSDL2 -lSDL2_ttf -lSDL2_image -c LTexture.cpp
Platform.o: Platform.cpp
	g++ -lSDL2 -lSDL2_image -c Platform.cpp
Ball.o: Ball.cpp
	g++ -lSDL2 -lSDL2_image -c Ball.cpp
Box.o: Box.cpp
	g++ -lSDL2 -lSDL2_image -c Box.cpp
Brick.o: Brick.cpp
	g++ -lSDL2 -lSDL2_image -c Brick.cpp
LTimer.o: LTimer.cpp
	g++ -lSDL2 -lSDL2_image -c LTimer.cpp

clean: 
	rm dxBall *.o
