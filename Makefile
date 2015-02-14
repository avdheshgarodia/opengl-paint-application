

CC=g++
exe=paint

all:	
	$(CC) -Wno-deprecated lodepng.cpp paint.cpp -o $(exe) -framework OpenGL -framework GLUT
