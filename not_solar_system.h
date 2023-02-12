#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "math.h"

#include "objects3D.h"


class NotPlanet{
public:
    NotPlanet(Object3D* object3D, double self_omega, double orbital_radius, double orbital_omega, double self_radius=1):
    object3D(object3D), self_omega(self_omega), orbital_radius(orbital_radius), orbital_omega(orbital_omega), self_radius(self_radius){

    }

    void render(
        const glm::mat4& M,
        const glm::mat4& V,
        const glm::mat4& P)const{

        this->object3D->render(M * this->matrix, V, P,this->matrix * this->light_matrix);
    }

    void updateRecursive(double dt, const glm::mat4& parent_orbital_matrix=glm::mat4(1.f));
    std::vector<NotPlanet*>satellites;
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
    glm::mat4 light_matrix = glm::mat4(1.f);
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

    void render(
        const glm::mat4& M,
        const glm::mat4& V,
        const glm::mat4& P)const;

    void xD();

private:
    NotPlanet * moon, *sun;
    bool _xd = false;
    const Sphere sphere;
    const SimpleTextureShader simpleTextureShader;
};