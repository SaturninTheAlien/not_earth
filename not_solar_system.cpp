#include "not_solar_system.h"


#include <glm/gtc/matrix_transform.hpp>


void NotPlanet::update(double dt){

    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.f), float(this->self_phi), glm::vec3(0.f,1.f,0.f));
    
    this->self_phi += this->self_omega * dt;
    if(this->self_phi > 2*M_PI){
        this->self_phi -= 2*M_PI;
    }
    else if(this->self_phi < 0){
        this->self_phi += 2*M_PI;
    }

    glm::mat4 axial_tilt_matrix = glm::rotate(glm::mat4(1.f), float(glm::radians(axial_tilt)), glm::vec3(0.f,0.f,1.f));

    for(NotPlanet* np: this->satellites){
        np->update(dt);

        np->orbital_matrix = glm::translate(glm::mat4(1.f),
            glm::vec3(np->orbital_radius * cos(np->orbital_phi),
            0.f,
            np->orbital_radius * sin(np->orbital_phi)));

        np->orbital_matrix = this->orbital_matrix * np->orbital_matrix;

        np->orbital_phi -= np->orbital_omega * dt;
        if(np->orbital_phi > 2*M_PI){
            np->orbital_phi -= 2*M_PI;
        }
        else if(np->orbital_phi < 0){
            np->orbital_phi += 2*M_PI;
        }
    }

    this->matrix = glm::scale(this->orbital_matrix * axial_tilt_matrix * rotation_matrix,
    glm::vec3(this->self_radius, this->self_radius, this->self_radius));
}

NotSolarSystem::NotSolarSystem():
sphere(){

    this->objects3D.push_back(new Sun(this->sphere, this->simpleTextureShader));
    this->objects3D.push_back(new Earth(this->sphere));
    this->objects3D.push_back(new Moon(this->sphere));
    this->objects3D.push_back(new Ksienrzyc(this->simpleTextureShader));

    this->sun = new NotPlanet(this->objects3D[0], 0,  0, 0, 3);
    NotPlanet* earth = new NotPlanet(this->objects3D[1], 1./10 , 15, 1./365.25, 1, 23.44);
    this->moon = new NotPlanet(NotPlanet(this->objects3D[2], 1./27.3, 2, 1./27.3, 0.2731, 6.687));
    

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
    //this->sun->updateRecursive(dt);
    this->sun->update(dt);
}

void NotSolarSystem::render(const glm::mat4& MVP)const{
    
    for(const NotPlanet* np:this->not_planets){
        np->render(MVP);
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