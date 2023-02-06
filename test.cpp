#include <iostream>
#include <sstream>
#include <stdexcept>


// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <pybind11/embed.h>

#include "objects3D.h"

namespace py = pybind11;

int main(int argc, char**argv){

    py::scoped_interpreter guard{};

    if( !glfwInit() ){
        throw std::runtime_error("Failed to initialize GLFW\n");
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow( 1280, 800, "Not Earth!", 0, 0);

    glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to initialize GLEW\n");
	}

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    if(!GLEW_VERSION_3_3){
        std::ostringstream os;
        os<<"Modern opengl not supported!"<<std::endl;
        os<<"Please install some drivers or don't use an antique computer!"<<std::endl;
        throw std::runtime_error(os.str());
    }

    glEnable(GL_TEXTURE_2D);
    glEnable (GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    int selected_model = 0;

    glm::mat4 M, V, P;

    P = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    V = glm::lookAt(
        glm::vec3(4,0,0), // Camera is at (4,3,3), in World Space
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

    int selected_object_id = 1;

    do{
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0,0,0.25, 1);

		objects3D[selected_object_id]->render(M,V,P, lightPosition);

		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

    /*SDL_ShowWindow(window);
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
    }*/

    glfwTerminate();

    return 0;
}