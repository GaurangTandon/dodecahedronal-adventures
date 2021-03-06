all: core

core: main.cpp
	gww -pthread -o main ../glad.c main.cpp -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl

clean:
	rm main
