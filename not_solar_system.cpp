#include "not_solar_system.h"


#include <glm/gtc/matrix_transform.hpp>


void NotPlanet::updateRecursive(double dt, const glm::mat4& parent_orbital_matrix){

    glm::mat4 rotation_matrix = glm::mat4(1.f);

    this->self_phi += this->self_omega * dt;
    if(this->self_phi > 2*M_PI){
        this->self_phi -= 2*M_PI;
    }
    else if(this->self_phi < 0){
        this->self_phi += 2*M_PI;
    }

    rotation_matrix = glm::rotate(rotation_matrix, float(this->self_phi), glm::vec3(0.f,1.f,0.f));

    glm::mat4 orbital_matrix = glm::mat4(1.f);

    if(this->orbital_radius>0){
        this->orbital_phi -= this->orbital_omega * dt;
        
        if(this->orbital_phi > 2*M_PI){
            this->orbital_phi -= 2*M_PI;
        }
        else if(this->orbital_phi < 0){
            this->orbital_phi += 2*M_PI;
        }

        orbital_matrix = glm::translate(orbital_matrix,
            glm::vec3(this->orbital_radius * cos(this->orbital_phi),
            0.f,
            this->orbital_radius * sin(this->orbital_phi)));
          
        orbital_matrix = parent_orbital_matrix * orbital_matrix;

        this->light_matrix = glm::rotate(glm::mat4(1.0f), float(this->orbital_phi), glm::vec3(0.0f, 1.0f, 0.0f));  
    }

    for(NotPlanet* np: this->satellites){
        np->updateRecursive(dt, orbital_matrix);
    }

    this->matrix = glm::scale(orbital_matrix * rotation_matrix,
    glm::vec3(this->self_radius, this->self_radius, this->self_radius));

    //this->normal_matrix = glm::rotate(this->normal_matrix, glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

NotSolarSystem::NotSolarSystem():
sphere(){

    this->objects3D.push_back(new Sun(this->sphere, this->simpleTextureShader));
    this->objects3D.push_back(new Earth(this->sphere));
    this->objects3D.push_back(new Moon(this->sphere));
    this->objects3D.push_back(new Ksienrzyc(this->simpleTextureShader));

    this->sun = new NotPlanet(this->objects3D[0], 1./20,  0, 0);
    NotPlanet* earth = new NotPlanet(this->objects3D[1], 1./10 , 5, 1./365.25);
    this->moon = new NotPlanet(NotPlanet(this->objects3D[2], 1./27.3, 2, 1./27.3, 0.2731));
    

    sun->satellites.emplace_back(earth);
    earth->satellites.emplace_back(moon);

    this->not_planets.emplace_back(sun);
    this->not_planets.emplace_back(earth);
    this->not_planets.emplace_back(moon);
}

NotSolarSystem::~NotSolarSystem(){
    for(Object3D*& o: this->objects3D){
        if(o!=nullptr){
            delete o;
            o = nullptr;
        }
    }

    for(NotPlanet*& np: this->not_planets){
        if(np!=nullptr){
            delete np;
            np = nullptr;
        }
    }
}

void NotSolarSystem::update(double dt){
    this->sun->updateRecursive(dt);
}

void NotSolarSystem::render(
        const glm::mat4& M,
        const glm::mat4& V,
        const glm::mat4& P)const{
    
    for(const NotPlanet* np:this->not_planets){
        np->render(M,V,P);
    }
}

void NotSolarSystem::xD(){
    this-> _xd = !this->_xd;
    if(this->_xd){
        this->moon->object3D = this->objects3D[3];
        this->moon->self_radius = 1;
    }
    else{
        this->moon->object3D = this->objects3D[2];
        this->moon->self_radius = 0.2731;
    }
}