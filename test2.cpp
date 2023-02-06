#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <pybind11/embed.h>

#include "objects3D.h"

namespace py = pybind11;

class NotSolarSystemApp{
public:
    NotSolarSystemApp();
    ~NotSolarSystemApp();
    std::vector<Object3D* > objects;

    void update();
    void render(
        const glm::mat4& M,
        const glm::mat4& V,
        const glm::mat4& P)const;

    bool muneInsteadOfTheMoon = false;
    double omega0 = 0.025;

protected:

    glm::mat4 moon_rotation = glm::mat4(1.0f);


    glm::mat4 earth_orbital_rotation = glm::mat4(1.0f);
    glm::mat4 earth_rotation = glm::mat4(1.0f);

    glm::mat4 earth_matrix = glm::mat4(1.0f);
    glm::mat4 moon_matrix = glm::mat4(1.0f);
    glm::mat4 mune_matrix = glm::mat4(1.0f);


    glm::vec3 lightPosition = glm::vec3(110.f, 0.f, 0.0f);

    
    double earth_orbit_radius = 5;
    double moon_orbit_radius = 2;

    double moon_size = 0.2731;
    double sun_size = 5;

private:
    const Sphere sphere;
    const SimpleTextureShader simpleTextureShader;
};


NotSolarSystemApp::NotSolarSystemApp():
sphere(){
    objects.push_back(new Sun(this->sphere, this->simpleTextureShader));
    objects.push_back(new Earth(this->sphere));
    objects.push_back(new Moon(this->sphere));
    objects.push_back(new Ksienrzyc(this->simpleTextureShader));

    //this->earth_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,earth_orbit_radius));


    this->earth_matrix[3][2] = earth_orbit_radius;
    //this->earth_matrix = this->earth_orbital_rotation;
}

NotSolarSystemApp::~NotSolarSystemApp(){
    for(Object3D*& o: this->objects){
        if(o!=nullptr){
            delete o;
            o = nullptr;
        }
    }
    objects.clear();
}


void NotSolarSystemApp::update(){

    this->earth_orbital_rotation = glm::rotate(this->earth_orbital_rotation, float(this->omega0/365.25), glm::vec3(0.0f, 1.0f, 0.0f));
    this->earth_rotation = glm::rotate(this->earth_rotation, float(this->omega0/10), glm::vec3(0.0f, 1.0f, 0.0f));
    
    this->moon_rotation = glm::rotate(this->moon_rotation, float(this->omega0/27.3), glm::vec3(0.0f, 1.0f, 0.0f));


    glm::mat4 earth_orbital = glm::translate(this->earth_orbital_rotation, glm::vec3(-this->earth_orbit_radius, 0.f, 0.f));
    glm::mat4 earth_axial_tilt = glm::rotate(glm::mat4(1.0f), float(glm::radians(23.4393)), glm::vec3(1.0f, 0.0f, 0.0f));
    this->earth_matrix = earth_orbital*earth_axial_tilt* this->earth_rotation;

    //glm::mat4 moon_orbital = this->moon_rotation;

    glm::mat4 moon_orbital = glm::rotate(glm::mat4(1.0f), float(glm::radians(5.145396)), glm::vec3(1.f, 0.f, 0.f)) * this->moon_rotation;
    moon_orbital = glm::translate(moon_orbital, glm::vec3(-this->moon_orbit_radius, 0.f, 0.0f));

    moon_orbital = earth_orbital * moon_orbital;

    this->mune_matrix = moon_orbital;

    this->moon_matrix = glm::scale(moon_orbital, glm::vec3(this->moon_size, this->moon_size, this->moon_size));
}

void NotSolarSystemApp::render(
        const glm::mat4& M,
        const glm::mat4& V,
        const glm::mat4& P)const{

    //render Sun
    this->objects[0]->render(M,V,P, this->lightPosition);

    //render Earth
    this->objects[1]->render(M*this->earth_matrix, V, P, this->lightPosition);

    if(this->muneInsteadOfTheMoon){
        //render Mune
        this->objects[3]->render(M*this->mune_matrix, V, P, this->lightPosition);
    }
    else{
        //render Moon
        this->objects[2]->render(M*this->moon_matrix, V, P, this->lightPosition);
    }
}

NotSolarSystemApp * app = nullptr;
glm::mat4 M;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS){
        if(key == GLFW_KEY_ESCAPE){
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        else if(key == GLFW_KEY_Z){
            M = glm::mat4(1.0f);
            app->omega0 = 0.025;
            app->muneInsteadOfTheMoon = false;
        }
        else if(key == GLFW_KEY_W){
            app->omega0 *= 2;
        }
        else if(key == GLFW_KEY_S){
            app->omega0 /= 2;
        }
        else if(key == GLFW_KEY_M){
            app->muneInsteadOfTheMoon = !app->muneInsteadOfTheMoon;
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

    glm::mat4 V, P;

    P = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    V = glm::lookAt(
        glm::vec3(0,0,20), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    M = glm::mat4(1.0f);

    app = new NotSolarSystemApp();

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

        app->update();

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0,0,0.25, 1);

        app->render(M,V,P);

		glfwSwapBuffers(window);
    }

    delete app;

    glfwTerminate();

    return 0;
}