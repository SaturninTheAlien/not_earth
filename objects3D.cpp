#include "objects3D.h"
#include <GL/glew.h>

Moon::Moon(const Model& sphere_model):
sphere_model(sphere_model){

    this->texture = loadTextureSDL("textures/moon.jpg");
    this->texture_normal = loadTextureSDL("textures/moon_normal.jpg");

    this->shader = compileShader(readFile("shaders/moon.vert.glsl"),
    readFile("shaders/moon.frag.glsl"));

    this->shader_M_id = getUniformID(shader, "M");
    this->shader_V_id = getUniformID(shader, "V");
    this->shader_P_id = getUniformID(shader, "P");

    this->shader_image_id = getUniformID(shader, "image");
    this->shader_image_normal_id = getUniformID(shader, "image_normals");
}

void Moon::render(const glm::mat4& M, const glm::mat4& V, const glm::mat4& P, const glm::vec3& light_dir)const{
    glUseProgram(this->shader);

    glUniformMatrix4fv(this->shader_M_id, 1, GL_FALSE, &M[0][0]);
    glUniformMatrix4fv(this->shader_V_id, 1, GL_FALSE, &V[0][0]);
    glUniformMatrix4fv(this->shader_P_id, 1, GL_FALSE, &P[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glUniform1i(this->shader_image_id, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->texture_normal);
    glUniform1i(this->texture_normal, 1);

    this->sphere_model.draw();
}



Ksienrzyc::Ksienrzyc():
model("models/ksienrzyc.obj", true){

    this->texture = loadTextureSDL("models/ksienrzyc.png");
    this->shader = compileShader(readFile("shaders/simple_texture.vert.glsl"), readFile("shaders/simple_texture.frag.glsl"));
    this->shader_mvp_id = getUniformID(shader, "MVP");
    this->shader_texture_id = getUniformID(shader, "image");
}

void Ksienrzyc::render(const glm::mat4& M, const glm::mat4& V, const glm::mat4& P, const glm::vec3& light_dir)const{
    glUseProgram(this->shader);

    glm::mat4 MVP = P * V * M;
    glUniformMatrix4fv(this->shader_mvp_id, 1, GL_FALSE, &MVP[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glUniform1i(this->shader_texture_id, 0);

    this->model.draw();
}