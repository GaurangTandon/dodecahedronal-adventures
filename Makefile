all: core

core: main.cpp
	gww --std=c++17 -pthread -o main ../glad.c main.cpp -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl -fsanitize=address -fsanitize=undefined -g

clean:
	rm main
