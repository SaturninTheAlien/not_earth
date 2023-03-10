
UNAME_S = $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CFLAGS = -std=c++17 -I/opt/homebrew/include $(shell python3-config --includes)
	LDFLAGS = -L/opt/homebrew/lib -lglfw -lGLEW -lassimp $(shell python3-config --ldflags --embed) -framework OpenGL

else
	CFLAGS = -std=c++17 $(shell python3-config --includes)
	LDFLAGS = -lglfw -lGLEW -lassimp $(shell python3-config --ldflags --embed) -lGL

endif

common_objects = utils.o shader.o model.o model_loader.o texture.o objects3D.o not_solar_system.o skybox.o compass_rose.o
headers1 = utils.h shader.h model.h model_loader.h texture.h

not_earth: $(common_objects) main.o
	c++ main.o $(common_objects) $(LDFLAGS) -o not_earth.x

run: not_earth
	./not_earth.x

all: not_earth test test3

test: $(common_objects) test.o
	c++ test.o $(common_objects) $(LDFLAGS) -o test.x

test3: $(common_objects) test3.o
	c++ test3.o $(common_objects) $(LDFLAGS) -o test3.x 

main.o: main.cpp *.h
	c++ $(CFLAGS) main.cpp -c -o main.o

test.o: test.cpp *.h
	c++ $(CFLAGS) test.cpp -c -o test.o

test3.o: test3.cpp *.h
	c++ $(CFLAGS) test3.cpp -c -o test3.o


utils.o: utils.h utils.cpp
	c++ $(CFLAGS) utils.cpp -c -o utils.o

shader.o: utils.h shader.h shader.cpp
	c++ $(CFLAGS) shader.cpp -c -o shader.o

model.o: utils.h model_loader.h model.h model.cpp
	c++ $(CFLAGS) model.cpp -c -o model.o

model_loader.o: utils.h model_loader.h model_loader.cpp
	c++ $(CFLAGS) model_loader.cpp -c -o model_loader.o

texture.o: utils.h texture.h texture.cpp
	c++ $(CFLAGS) texture.cpp -c -o texture.o

not_solar_system.o: objects3D.h not_solar_system.cpp not_solar_system.h $(headers1)
	c++ $(CFLAGS) not_solar_system.cpp -c -o not_solar_system.o

objects3D.o: objects3D.cpp objects3D.h $(headers1)
	c++ $(CFLAGS) objects3D.cpp -c -o objects3D.o

compass_rose.o: compass_rose.cpp compass_rose.h $(headers1)
	c++ $(CFLAGS) compass_rose.cpp -c -o compass_rose.o

skybox.o: skybox.cpp skybox.h $(headers1)
	c++ $(CFLAGS) skybox.cpp -c -o skybox.o

clean:
	rm -f *.x *.o

clean_shit:
	rm -f *.o

.PHONY: not_earth test test3 run all clean clean_shit


