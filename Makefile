
UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CFLAGS = -std=c++17 -I/opt/homebrew/include $(shell python3-config --includes)
	LDFLAGS = -L/opt/homebrew/lib -lglfw -lGLEW -lassimp $(shell python3-config --ldflags --embed) -framework OpenGL

else
	CFLAGS = -std=c++17 $(shell python3-config --includes)
	LDFLAGS = -lglfw -lGLEW -lassimp $(shell python3-config --ldflags --embed) -lGL

endif

not_earth:
	c++ $(CFLAGS) main.cpp utils.cpp shader.cpp model.cpp model_loader.cpp texture.cpp objects3D.cpp  not_solar_system.cpp $(LDFLAGS) -o not_earth.x

test:
	c++ $(CFLAGS) test.cpp utils.cpp shader.cpp model.cpp model_loader.cpp texture.cpp objects3D.cpp not_solar_system.cpp $(LDFLAGS) -o test.x

test2:
	c++ $(CFLAGS) test2.cpp utils.cpp shader.cpp model.cpp model_loader.cpp texture.cpp objects3D.cpp $(LDFLAGS) -o test.x