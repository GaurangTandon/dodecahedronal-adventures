main: main.cpp camera.h cube.h hexa_bipyramid.h polyhedron.h regular_dodec.h shader.h unidecagon.h shader.vs shader.fs
	g++ --std=c++17 -pthread -o main ../glad.c main.cpp -lglfw -lGLU -lGL -lXrandr -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl -fsanitize=address -fsanitize=undefined -g

clean:
	rm main
