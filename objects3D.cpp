#include "objects3D.h"
#include <GL/glew.h>

Moon::Moon(const Model& sphere_model):
sphere_model(sphere_model){

    this->texture = load_texture("textures/moon.jpg");
    this->texture_normal = load_texture("textures/moon_normal.jpg");

    this->shader = compileShader(readFile("shaders/moon.vert.glsl"),
    readFile("shaders/moon.frag.glsl"));

    this->shader_light_position_id = getUniformID(shader, "lightPosition");

    this->shader_M_id = getUniformID(shader, "M");
    this->shader_V_id = getUniformID(shader, "V");
    this->shader_P_id = getUniformID(shader, "P");

    this->shader_image_id = getUniformID(shader, "image");
    this->shader_image_normal_id = getUniformID(shader, "image_normals");
}

void Moon::render(const glm::mat4& M, const glm::mat4& V, const glm::mat4& P, const glm::vec3& lightPosition)const{
    glUseProgram(this->shader);

    glUniform3f(this->shader_light_position_id, lightPosition.x, lightPosition.y, lightPosition.z);

    glUniformMatrix4fv(this->shader_M_id, 1, GL_FALSE, &M[0][0]);
    glUniformMatrix4fv(this->shader_V_id, 1, GL_FALSE, &V[0][0]);
    glUniformMatrix4fv(this->shader_P_id, 1, GL_FALSE, &P[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glUniform1i(this->shader_image_id, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->texture_normal);
    glUniform1i(this->shader_image_normal_id, 1);

    this->sphere_model.draw();
}


Earth::Earth(const Model& sphere_model):
sphere_model(sphere_model){

    this->texture = load_texture("textures/2k_earth_daymap.jpg");
    this->texture_normal = load_texture("textures/2k_earth_normal_map.tif");
    this->texture_night = load_texture("textures/2k_earth_nightmap.jpg");
    this->texture_spec = load_texture("textures/2k_earth_specular_map.tif");

    this->shader = compileShader(readFile("shaders/moon.vert.glsl"),
    readFile("shaders/earth.frag.glsl"));

    this->shader_light_position_id = getUniformID(shader, "lightPosition");

    this->shader_M_id = getUniformID(shader, "M");
    this->shader_V_id = getUniformID(shader, "V");
    this->shader_P_id = getUniformID(shader, "P");

    this->shader_image_id = getUniformID(shader, "image");
    this->shader_image_night_id = getUniformID(shader, "image_night");
    this->shader_image_spec_id = getUniformID(shader, "image_spec");
    //this->shader_image_normal_id = getUniformID(shader, "image_normals");
}


void Earth::render(const glm::mat4& M, const glm::mat4& V, const glm::mat4& P, const glm::vec3& lightPosition)const{
    glUseProgram(this->shader);

    glUniform3f(this->shader_light_position_id, lightPosition.x, lightPosition.y, lightPosition.z);

    glUniformMatrix4fv(this->shader_M_id, 1, GL_FALSE, &M[0][0]);
    glUniformMatrix4fv(this->shader_V_id, 1, GL_FALSE, &V[0][0]);
    glUniformMatrix4fv(this->shader_P_id, 1, GL_FALSE, &P[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glUniform1i(this->shader_image_id, 0);

    /*glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->texture_normal);
    glUniform1i(this->shader_image_normal_id, 1);*/

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, this->texture_spec);
    glUniform1i(this->shader_image_spec_id, 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, this->texture_night);
    glUniform1i(this->shader_image_night_id, 3);

    this->sphere_model.draw();
}

Ksienrzyc::Ksienrzyc(const SimpleTextureShader&shader): shader(shader),
model("models/ksienrzyc.obj", true){

    this->texture = load_texture("models/ksienrzyc.png");
    /*this->shader = compileShader(readFile("shaders/simple_texture.vert.glsl"), readFile("shaders/simple_texture.frag.glsl"));
    this->shader_mvp_id = getUniformID(shader, "MVP");
    this->shader_texture_id = getUniformID(shader, "image");*/
}

void Ksienrzyc::render(const glm::mat4& M, const glm::mat4& V, const glm::mat4& P, const glm::vec3& lightPosition)const{
    glm::mat4 MVP = P * V * M;

    this->shader.use(MVP, this->texture);
    this->model.draw();
}

Sun::Sun(const Model & sphere_model, const SimpleTextureShader&shader):
sphere_model(sphere_model), shader(shader){
    texture = load_texture("textures/2k_sun.jpg");
}

void Sun::render(const glm::mat4& M, const glm::mat4& V, const glm::mat4& P, const glm::vec3& lightPosition)const{

    glm::mat4 MVP = P * V * M;

    this->shader.use(MVP, this->texture);
    this->sphere_model.draw();
}