
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CFLAGS = -std=c++17 -I/opt/homebrew/include $(shell python3-config --include)
	LDFLAGS = -L/opt/homebrew/lib -lglfw -lGLEW -lassimp $(shell python3-config --ldflags --embed) -framework OpenGL

else
	CFLAGS = -std=c++17 $(shell python3-config --include)
	LDFLAGS = -lglfw -lGLEW -lassimp $(shell python3-config --ldflags --embed) -lGL

endif

not_earth:
	c++ $(CFLAGS) not_earth.cpp utils.cpp shader.cpp model.cpp model_loader.cpp texture.cpp objects3D.cpp $(LDFLAGS) -o not_earth.x

test:
	c++ $(CFLAGS) test.cpp utils.cpp shader.cpp model.cpp model_loader.cpp texture.cpp objects3D.cpp $(LDFLAGS) -o test.x