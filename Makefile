
# CFLAGS = -std=c++17
# LDFLAGS = -lGL -lGLEW -lSDL2 -lSDL2_image -lassimp

CFLAGS = -std=c++17 -I/opt/homebrew/include `python3-config --include`
LDFLAGS = -L/opt/homebrew/lib -lglfw -lGLEW -lassimp `python3-config --ldflags` -lpython3.10 -framework OpenGL

not_earth:
	c++ $(CFLAGS) not_earth.cpp utils.cpp shader.cpp model.cpp model_loader.cpp texture.cpp objects3D.cpp $(LDFLAGS) -o not_earth.x

test:
	c++ $(CFLAGS) test.cpp utils.cpp shader.cpp model.cpp model_loader.cpp texture.cpp objects3D.cpp $(LDFLAGS) -o test.x