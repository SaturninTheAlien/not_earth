#include "objects3D.h"
#include <GL/glew.h>

Moon::Moon(const Model& sphere_model):
sphere_model(sphere_model){

    this->texture = load_texture("textures/moon.jpg");

#ifdef USE_NORMALS_TEXTURES
    this->texture_normal = load_texture("textures/moon_normal.jpg");

    this->shader = compileShader(readFile("shaders/moon2.vert.glsl"),
    readFile("shaders/moon2.frag.glsl"));

    this->shader_image_normal_id = getUniformID(shader, "image_normal");

#else
    this->shader = compileShader(readFile("shaders/moon.vert.glsl"),
    readFile("shaders/moon.frag.glsl"));

#endif

    this->shader_MVP_id = getUniformID(shader, "MVP");
    this->shader_N_id = getUniformID(shader, "N");
    this->shader_light_position_id = getUniformID(shader, "lightPosition");

    this->shader_image_id = getUniformID(shader, "image");
}

void Moon::render(const glm::mat4& MVP, const glm::mat4& N, const glm::vec3& lightPosition)const{
    glUseProgram(this->shader);

    glUniform3f(this->shader_light_position_id, lightPosition.x, lightPosition.y, lightPosition.z);
    glUniformMatrix4fv(this->shader_MVP_id, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(this->shader_N_id, 1, GL_FALSE, &N[0][0]);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glUniform1i(this->shader_image_id, 0);


#ifdef USE_NORMALS_TEXTURES
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->texture_normal);
    glUniform1i(this->shader_image_normal_id, 1);
#endif

    this->sphere_model.draw();
}


Earth::Earth(const Model& sphere_model):
sphere_model(sphere_model){

    this->texture = load_texture("textures/2k_earth_daymap.jpg");
    this->texture_night = load_texture("textures/2k_earth_nightmap.jpg");
    this->texture_spec = load_texture("textures/2k_earth_specular_map.tif");

#ifdef USE_NORMALS_TEXTURES
    this->texture_normal = load_texture("textures/2k_earth_normal_map.tif");
    this->shader = compileShader(readFile("shaders/moon2.vert.glsl"),
    readFile("shaders/earth2.frag.glsl"));

    this->shader_image_normal_id = getUniformID(shader, "image_normal");
#else
    this->shader = compileShader(readFile("shaders/moon.vert.glsl"),
    readFile("shaders/earth.frag.glsl"));
#endif
    this->shader_MVP_id = getUniformID(shader, "MVP");
    this->shader_N_id= getUniformID(shader, "N");
    this->shader_light_position_id = getUniformID(shader, "lightPosition");

    this->shader_image_id = getUniformID(shader, "image");
    this->shader_image_night_id = getUniformID(shader, "image_night");
    this->shader_image_spec_id = getUniformID(shader, "image_spec");
}


void Earth::render(const glm::mat4& MVP, const glm::mat4& N, const glm::vec3& lightPosition)const{
    glUseProgram(this->shader);

    glUniform3f(this->shader_light_position_id, lightPosition.x, lightPosition.y, lightPosition.z);

    glUniformMatrix4fv(this->shader_MVP_id, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(this->shader_N_id, 1, GL_FALSE, &N[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glUniform1i(this->shader_image_id, 0);

#ifdef USE_NORMALS_TEXTURES
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->texture_normal);
    glUniform1i(this->shader_image_normal_id, 1);
#endif

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
}

void Ksienrzyc::render(const glm::mat4& MVP, const glm::mat4& N, const glm::vec3& lightPosition)const{
    this->shader.use(MVP, this->texture);
    this->model.draw();
}

Sun::Sun(const Model & sphere_model, const SimpleTextureShader&shader):
sphere_model(sphere_model), shader(shader){
    texture = load_texture("textures/2k_sun.jpg");
}

void Sun::render(const glm::mat4& MVP, const glm::mat4& N, const glm::vec3& lightPosition)const{
    this->shader.use(MVP, this->texture);
    this->sphere_model.draw();
}