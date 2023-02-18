#pragma once

#include <glm/glm.hpp>

class Skybox{
public:
    Skybox();
    void render(const glm::mat4& MVP)const;
private:
    unsigned int shader = 0;
    unsigned int shader_mvp_id = 0;
    unsigned int shader_cubemap_id = 0;
    unsigned int vao = 0;
    unsigned int vertex_buffer = 0;
    unsigned int cubemap = 0;
};