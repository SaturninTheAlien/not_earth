#include <iostream>
#include <sstream>
#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <pybind11/embed.h>

#include "not_solar_system.h"
#include "compass_rose.h"
#include "skybox.h"

namespace py = pybind11;

double dt = 1./8;
glm::mat4 M, V, P;
NotSolarSystem * ns;
bool shouldRenderCompassRose = true;

bool shouldRotateWithMousePos = false;
glm::vec2 prevMousePos(0.f,0.f);

float viewRadius = 40.f;


glm::vec2 getRelativeMouseCursorPos(GLFWwindow* window){
    int width = 0, height = 0;
    glfwGetWindowSize(window, &width, &height);

    double xpos=0, ypos=0;
    glfwGetCursorPos(window, &xpos, &ypos);

    return glm::vec2(xpos/width, ypos/height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_ESCAPE){
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        else if(key == GLFW_KEY_Z){
            M = glm::mat4(1.0f);
        }
        else if(key == GLFW_KEY_X){
            M = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
        }
        else if(key == GLFW_KEY_Y){
            M = glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
        }
        else if(key == GLFW_KEY_M){
            ns->xD();
        }
        else if(key == GLFW_KEY_R){
            shouldRenderCompassRose = !shouldRenderCompassRose;
        }
        else if(key == GLFW_KEY_W){
            dt *= 2;
        }
        else if(key == GLFW_KEY_S){
            dt /= 2;
        }
    }
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){

    if(button == GLFW_MOUSE_BUTTON_MIDDLE){
        if(action == GLFW_PRESS){
            shouldRotateWithMousePos = true;
            prevMousePos = getRelativeMouseCursorPos(window);
        }
        else if(action == GLFW_RELEASE){
            shouldRotateWithMousePos = false;
        }
    }
}


int main(int argc, char**argv){

    {
        int i = 1;
        while(i<argc){
            std::string arg(argv[i]);
            if(arg=="-h" || arg=="--help"){
                std::cout<<"\"Not Earth\" art installation."<<std::endl;
                std::cout<<"For more details and controls open README file."<<std::endl;
                return 0;
            }
            ++i;
        }
    }

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
   
    if(!GLEW_VERSION_3_3){
        std::ostringstream os;
        os<<"Modern opengl not supported!"<<std::endl;
        os<<"Please install some drivers or don't use an antique computer!"<<std::endl;
        throw std::runtime_error(os.str());
    }


    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    P = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    V = glm::lookAt(
        glm::vec3(0,0,viewRadius), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    M = glm::mat4(1.0f);

    ns = new NotSolarSystem();

    CompassRose * cr = new CompassRose();
    Skybox * sb = new Skybox();

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    while(glfwWindowShouldClose(window) == 0 ){
        glfwPollEvents();
	    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f))*M;
        }
        else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f))*M;
        }

        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f))*M;
        }

        else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f))*M;
        }

        ns->update(dt);

        if(shouldRotateWithMousePos){
            glm::vec2 mousePos = getRelativeMouseCursorPos(window);
            if(mousePos!=prevMousePos){
                glm::vec2 rotationVec = mousePos - prevMousePos;
                prevMousePos = mousePos;

                M = glm::rotate(glm::mat4(1.0f),
                float(M_PI * glm::length(rotationVec)),
                glm::vec3(rotationVec.y, rotationVec.x, 0)) * M;
            }
        }       

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0,0,0.25, 1);

        glm::mat4 MVP = P*V*M;

        {
            glm::mat4 MVP_SB = glm::scale(MVP, glm::vec3(viewRadius, viewRadius, viewRadius));
            sb->render(MVP_SB);
        }

        glEnable(GL_DEPTH_TEST);
        ns -> render(MVP);

        glClear(GL_DEPTH_BUFFER_BIT);
        
        if(shouldRenderCompassRose){
            glClear(GL_DEPTH_BUFFER_BIT);

            float cr_size = 2;
            glm::mat4 MVP_CR = glm::scale(MVP, glm::vec3(cr_size, cr_size, cr_size));
            MVP_CR = glm::translate(glm::mat4(1.f), glm::vec3(0.75f, -0.75f, 0.f)) * MVP_CR;
            cr -> render(MVP_CR);
        }
        glDisable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
    }

    delete cr;
    delete ns;
    delete sb;

    glfwTerminate();

    return 0;
}