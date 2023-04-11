#! usr/bin/bash
g++ `pkg-config --cflags glfw3 glew` -o target Main.cpp Shader.cpp Mesh.cpp Window.cpp Camera.cpp `pkg-config --static --libs glfw3 glew`