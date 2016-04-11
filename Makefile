all: ballBounce

ballBounce: ballBounce.o LTexture.o
	g++ -lSDL2 -lSDL2_image ballBounce.o LTexture.o -o ballBounce

ballBounce.o: ballBounce.cpp
	g++ -lSDL2 -lSDL2_image -c ballBounce.cpp 
LTexture.o: LTexture.cpp
	g++ -lSDL2 -lSDL2_image -c LTexture.cpp

clean: 
	rm ballBounce *.o
