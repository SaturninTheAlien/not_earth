#pragma once
#include <glm/glm.hpp>
#include <vector>

class NotPlanet{
public:
    NotPlanet(const std::vector<NotPlanet*>&moons):
    moons(moons){

    }
    void update();
private:
    glm::mat4 orbital_rotation = glm::mat4(1.0f);
    glm::mat4 orbital_translation = glm::mat4(1.0f);
    glm::mat4 orbital_tilt = glm::mat4(1.0f);

    glm::mat4 self_rotation = glm::mat4(1.0f);

    std::vector<NotPlanet*>moons;
};