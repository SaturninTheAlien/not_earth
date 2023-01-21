
CFLAGS = -std=c++17
LDFLAGS = -lGL -lGLEW -lSDL2 -lSDL2_image -lassimp

all:
	c++ $(CFLAGS) main.cpp utils.cpp shader.cpp model.cpp model_loader.cpp texture.cpp objects.cpp $(LDFLAGS) -o not_earth.x