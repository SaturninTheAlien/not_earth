#include "compass_rose.h"

#include <GL/glew.h>
#include <glm/glm.hpp>


#include "utils.h"
#include "shader.h"
#include "texture.h"

CompassRose::CompassRose(){
    this->texture_x = load_texture("textures/compass_rose/x.png");
    this->texture_y = load_texture("textures/compass_rose/y.png");
    this->texture_z = load_texture("textures/compass_rose/z.png");

    this->simple_color_shader = compileShader(
        readFile("shaders/simple_color.vert.glsl"),
        readFile("shaders/simple_color.frag.glsl")
    );

    this->simple_color_shader_mvp_id = getUniformID(this->simple_color_shader, "MVP");

    this->textured_point_shader = compileShader(
        readFile("shaders/simple_color.vert.glsl"),
        readFile("shaders/textured_point.frag.glsl")
    );

    this->textured_point_shader_mvp_id = getUniformID(this->textured_point_shader, "MVP");
    this->textured_point_shader_image_id = getUniformID(this->textured_point_shader, "image");

    float vertices[] = {
        0.f, 0.f, 0.f,
        1.f, 0.f, 0.f,

        0.f, 0.f, 0.f,
        0.f, 1.f, 0.f,

        0.f, 0.f, 0.f,
        0.f, 0.f, 1.f
    };

    float colors[] = {
        1.f, 0.f, 0.f,
        1.f, 0.f, 0.f,

        0.f, 1.f, 0.f,
        0.f, 1.f, 0.f,

        0.25f, 0.25f, 1.f,
        0.25f, 0.25f, 1.f
    };

    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vertex_buffer);
    glGenBuffers(1, &this->color_buffer);

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, this->color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);    

    glBindVertexArray(0);    
}

void CompassRose::render(const glm::mat4 &MVP)const{

    glBindVertexArray(this->vao);

    glUseProgram(this->simple_color_shader);
    glUniformMatrix4fv(this->simple_color_shader_mvp_id, 1, GL_FALSE, &MVP[0][0]);
    
    glDrawArrays(GL_LINES, 0, 6);


    glPointSize(16);

    glUseProgram(this->textured_point_shader);
    glUniformMatrix4fv(this->textured_point_shader_mvp_id, 1, GL_FALSE, &MVP[0][0]);
    glUniform1i(this->textured_point_shader_image_id, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture_x);
    
    glDrawArrays(GL_POINTS, 1, 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture_y);

    glDrawArrays(GL_POINTS, 3, 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture_z);

    glDrawArrays(GL_POINTS, 5, 1);

    glBindVertexArray(0);
}