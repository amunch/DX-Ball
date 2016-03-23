all: ballBounce

ballBounce: ballBounce.cpp
	g++ -lSDL -lSDL2_image ballBounce.cpp -o ballBounce

clean: 
	rm ballBounce
