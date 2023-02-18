#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "math.h"

#include "objects3D.h"




class NotPlanet{
public:
    NotPlanet(Object3D* object3D, double self_omega, double orbital_radius,
    double orbital_omega, double self_radius=1, double axial_tilt=0):
    object3D(object3D),
    self_omega(self_omega),
    orbital_radius(orbital_radius),
    orbital_omega(orbital_omega),
    self_radius(self_radius),
    axial_tilt(axial_tilt){
    }

    void render(const glm::mat4& MVP)const{
        this->object3D->render(MVP * this->matrix, this->matrix, glm::vec3(0.f,0.f,0.f));
    }
    void update(double dt);

    std::vector<NotPlanet*>satellites;

    double axial_tilt = 0;
    double self_phi = M_PI;
    double self_omega = 0;
    double self_radius = 1;

    double orbital_phi = 0;
    double orbital_omega = 0;
    double orbital_radius = 0;

    const glm::mat4 & getMatrix()const{
        return this->matrix;
    }

private:
    glm::mat4 matrix;
    glm::mat4 orbital_matrix = glm::mat4(1.f);
    
    Object3D* object3D;
    friend class NotSolarSystem;
};

class NotSolarSystem{
public:
    NotSolarSystem();
    ~NotSolarSystem();

    NotSolarSystem(const NotSolarSystem& source) = delete;
    NotSolarSystem(NotSolarSystem&& source) = delete;

    NotSolarSystem& operator=(const NotSolarSystem& source) = delete;
    NotSolarSystem& operator=(NotSolarSystem&& source) = delete;

    std::vector<Object3D* > objects3D;
    std::vector<NotPlanet* > not_planets;

    void update(double dt);

    void render(const glm::mat4& MVP)const;

    void xD();

private:
    NotPlanet * moon, *sun;
    bool _xd = false;
    const Sphere sphere;
    const SimpleTextureShader simpleTextureShader;
};