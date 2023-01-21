#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "objects3D.h"


int main(int argc, char**argv){

    SDL_Window * window = SDL_CreateWindow("Not Earth",0,0,1280,800, SDL_WINDOW_OPENGL|SDL_WINDOW_HIDDEN);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    {
        GLenum errorCode = glewInit();
        if(errorCode!=GLEW_OK){
            std::cerr<<"[GLEW error]"<<glewGetErrorString(errorCode)<<std::endl;
        }
    }

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

    if(!GLEW_VERSION_3_3){
        std::cerr<<"Modern opengl not supported!"<<std::endl;
        std::cerr<<"Please install some drivers!"<<std::endl;
        return 1;
    }


    glEnable(GL_TEXTURE_2D);
    glEnable (GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    int selected_model = 0;

    glm::mat4 M, V, P;

    P = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    V = glm::lookAt(
        glm::vec3(10,0,0), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    M = glm::mat4(1.0f);

    glm::vec3 lightPosition = glm::vec3(100.f, 10.f, 100.f);

    Sphere sphere;
    SimpleTextureShader simpleTextureShader;

    std::vector<Object3D*> objects3D;

    Moon moon(sphere);
    Earth earth(sphere);

    Sun sun(sphere, simpleTextureShader);
    Ksienrzyc ksienrzyc(simpleTextureShader);
    
    objects3D.push_back(&moon);
    objects3D.push_back(&earth);
    objects3D.push_back(&sun);
    objects3D.push_back(&ksienrzyc);

    int selected_object_id = 0;

    SDL_ShowWindow(window);
    bool quit = false;
    while(!quit){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT){
                quit = true;
            }

            else if(event.type == SDL_KEYDOWN && event.key.repeat == 0){
                if (event.key.keysym.sym == SDLK_ESCAPE){
					quit = true;
				}
                else if (event.key.keysym.sym == SDLK_z){
					M = glm::mat4(1.0f);
				}
                else if(event.key.keysym.sym == SDLK_o){
                    selected_object_id = (selected_object_id + 1) % objects3D.size();
                }
            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	    if (currentKeyStates[SDL_SCANCODE_RIGHT]){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f))*M;
        }
        else if(currentKeyStates[SDL_SCANCODE_LEFT]){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f))*M;
        }

        if(currentKeyStates[SDL_SCANCODE_UP]){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f))*M;
        }

        else if(currentKeyStates[SDL_SCANCODE_DOWN]){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(0.0f, 0.0f, 1.0f))*M;
        }

        if(currentKeyStates[SDL_SCANCODE_L]){
            glm::mat4 RM = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            glm::vec4 v = glm::vec4(lightPosition.x, lightPosition.y, lightPosition.z, 1);
            v = RM * v;

            lightPosition = glm::vec3(v.x, v.y, v.z);            
        }

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0,0,0.25, 1);

        objects3D[selected_object_id]->render(M,V,P, lightPosition);
        //moon.render(M, V, P, light_dir);
        //ksienrzyc.render(M, V, P, light_dir);

        SDL_GL_SwapWindow(window);
    }

    return 0;
}