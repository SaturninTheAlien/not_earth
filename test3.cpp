
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//using namespace glm;
#include "utils.h"
#include "shader.h"

#include <pybind11/embed.h>

glm::mat4 M(1.f);
//int selected_object_id = 1;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_ESCAPE){
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        else if(key == GLFW_KEY_Z){
            M = glm::mat4(1.0f);
        }
        /*else if(key == GLFW_KEY_O){
            selected_object_id = (selected_object_id + 1) % ns->objects3D.size();
        }*/
    }
}

int main( int argc, char**argv ){
    pybind11::scoped_interpreter guard{};
	// Initialise GLFW
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

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	
    bool quit = false;

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; 

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    //
    glBindVertexArray(0);

    unsigned int shader = compileShader(
        readFile("shaders/simple_color.vert.glsl"),
        readFile("shaders/simple_color.frag.glsl"));
    
    int shader_matrix_id = getUniformID(shader, "MVP");
    int shader_color_id = getUniformID(shader, "color");


    //unsigned int star_texture = load_texture("textures/star.png");

    glm::mat4 MVP(1.0f);
    //MVP = glm::scale(MVP, glm::vec3(0.5f, 0.5f, 0.5f));

    glfwSetKeyCallback(window, key_callback);

    while(glfwWindowShouldClose(window) == 0 ){
        glfwPollEvents();
	    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(0.0f, 0.0f, 1.0f))*M;
        }
        else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f))*M;
        }

        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f))*M;
        }

        else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f))*M;
        }

        glClear( GL_COLOR_BUFFER_BIT );

        glClearColor(0,0,0.25, 1);
        glUseProgram(shader);
        glUniformMatrix4fv(shader_matrix_id, 1, GL_FALSE, &MVP[0][0]);
        glUniform3f(shader_color_id, 0.f, 1.f, 0.f);

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        glPointSize(10);
        glDrawArrays(GL_POINTS, 0, 3);
        //glLineWidth(3);
        //glDrawArrays(GL_LINE_LOOP, 0, 3);
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

    }

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}