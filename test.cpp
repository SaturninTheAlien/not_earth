#include <iostream>
#include <sstream>
#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <pybind11/embed.h>

#include "not_solar_system.h"

namespace py = pybind11;

glm::mat4 M;
int selected_object_id = 1;
NotSolarSystem * ns;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_ESCAPE){
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        else if(key == GLFW_KEY_Z){
            M = glm::mat4(1.0f);
        }
        else if(key == GLFW_KEY_O){
            selected_object_id = (selected_object_id + 1) % ns->objects3D.size();
        }
    }
}

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
   
    //glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

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

    glm::mat4 V, P, N;

    P = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    V = glm::lookAt(
        glm::vec3(0,0,4), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    M = glm::mat4(1.0f);
    N = glm::mat4(1.0f);

    glm::vec3 lightPosition = glm::vec3(100.f, 10.f, 100.f);
    ns = new NotSolarSystem();

    glfwSetKeyCallback(window, key_callback);

    while(glfwWindowShouldClose(window) == 0 ){

        glfwPollEvents();		

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f))*M;
        }
        else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f))*M;
        }

        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f))*M;
        }

        else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f))*M;
        }

        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            N = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f))*N;       
        }

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0,0,0.25, 1);

		//ns->objects3D[selected_object_id]->render(P*V*M,N, lightPosition);
        ns->objects3D[selected_object_id]->render(P*V*M,M, lightPosition);
		
		glfwSwapBuffers(window);
	}

    delete ns;

    glfwTerminate();

    return 0;
}