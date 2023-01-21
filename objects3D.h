#pragma once

#include "shader.h"
#include "model.h"
#include "texture.h"
#include "utils.h"

class Object3D{
public:
    virtual ~Object3D(){}
    virtual void render(
        const glm::mat4& P,
        const glm::mat4& V,
        const glm::mat4& M,
        const glm::vec3& lightPosition
        )const = 0;
};

class Moon: public Object3D{
public:
    Moon(const Model& sphere_model);

    void render(
        const glm::mat4& P,
        const glm::mat4& V,
        const glm::mat4& M,
        const glm::vec3& lightPosition
        )const;
private:
    const Model & sphere_model;
    unsigned int texture = 0;
    unsigned int texture_normal = 0;
    int shader=0;
    int shader_light_position_id = -1;
    int shader_M_id=-1;
    int shader_V_id=-1;
    int shader_P_id=-1;
    int shader_image_id=-1;
    int shader_image_normal_id=-1;
};


class Earth: public Object3D{
public:
    Earth(const Model& sphere_model);

    void render(
        const glm::mat4& P,
        const glm::mat4& V,
        const glm::mat4& M,
        const glm::vec3& lightPosition
        )const;
protected:
    const Model & sphere_model;
    unsigned int texture = 0;
    unsigned int texture_normal = 0;
    unsigned int texture_night = 0;
    unsigned int texture_spec = 0;
    int shader=0;
    int shader_light_position_id = -1;
    int shader_M_id=-1;
    int shader_V_id=-1;
    int shader_P_id=-1;
    int shader_image_id=-1;
    int shader_image_night_id=-1;
    int shader_image_spec_id=-1;
    int shader_image_normal_id=-1;
    int shader_light_dir_id = -1;
};


class Ksienrzyc: public Object3D{
public:
    Ksienrzyc();
    void render(
        const glm::mat4& P,
        const glm::mat4& V,
        const glm::mat4& M,
        const glm::vec3& lightPosition
        )const;

private:
    Model model;
    unsigned int texture;
    int shader=0;
    int shader_mvp_id=-1;
    int shader_texture_id=-1;
};
