#include "skybox.h"

#include <GL/glew.h>

#include "utils.h"
#include "shader.h"
#include "texture.h"

Skybox::Skybox(){
    this->shader = compileShader(
        readFile("shaders/skybox.vert.glsl"),
        readFile("shaders/skybox.frag.glsl"));

    this->shader_mvp_id = getUniformID(this->shader, "MVP");
    this->shader_cubemap_id = getUniformID(this->shader, "cubemap");

    this->cubemap = load_cubemap({

        "textures/skybox/space_ft.png",
        "textures/skybox/space_bk.png",


        "textures/skybox/space_up.png",
        "textures/skybox/space_dn.png",
        
        "textures/skybox/space_rt.png",
        "textures/skybox/space_lf.png"     
    });

    /*float skyboxVertices[] = {
        -1.f, -1.f,  1.f,
         1.f, -1.f,  1.f,
         1,f, -1.f, -1.f, 

        -1.f, -1.f, -1.f,
        -1.f,  1.f,  1.f,
         1.f,  1.f,  1.f,


    }*/

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vertex_buffer);

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

void Skybox::render(const glm::mat4& MVP)const{
    glUseProgram(this->shader);
    glUniformMatrix4fv(this->shader_mvp_id, 1, GL_FALSE, &MVP[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemap);
    glUniform1i(this->shader_cubemap_id, 0);

    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);    
}