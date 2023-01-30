#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "objects3D.h"

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
        glm::vec3(0,0,20), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    M = glm::mat4(1.0f);

    //glm::vec3 lightPosition = glm::vec3(100.f, 10.f, 100.f);

    NotSolarSystemApp notSolarSystemApp;


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
                    notSolarSystemApp.omega0 = 0.025;
                    notSolarSystemApp.muneInsteadOfTheMoon = false;
				}
                else if(event.key.keysym.sym == SDLK_w){
                    notSolarSystemApp.omega0 *= 2;
                }
                else if(event.key.keysym.sym == SDLK_s){
                    notSolarSystemApp.omega0 /= 2;
                }
                else if(event.key.keysym.sym == SDLK_m){
                    notSolarSystemApp.muneInsteadOfTheMoon = !notSolarSystemApp.muneInsteadOfTheMoon;
                }

            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	    if (currentKeyStates[SDL_SCANCODE_RIGHT]){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(0.0f, 0.0f, 1.0f))*M;
        }
        else if(currentKeyStates[SDL_SCANCODE_LEFT]){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f))*M;
        }

        if(currentKeyStates[SDL_SCANCODE_UP]){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f))*M;
        }

        else if(currentKeyStates[SDL_SCANCODE_DOWN]){
            M = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f))*M;
        }

        /*if(currentKeyStates[SDL_SCANCODE_L]){
            glm::mat4 RM = glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            glm::vec4 v = glm::vec4(lightPosition.x, lightPosition.y, lightPosition.z, 1);
            v = RM * v;

            lightPosition = glm::vec3(v.x, v.y, v.z);            
        }*/

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0,0,0.25, 1);

        notSolarSystemApp.update();
        notSolarSystemApp.render(M,V,P);
        
        //notSolarSystemApp.objects[selected_object_id]->render(M,V,P, lightPosition);

        SDL_GL_SwapWindow(window);
    }

    return 0;
}