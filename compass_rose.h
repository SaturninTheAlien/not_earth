#pragma once
#include <glm/glm.hpp>

class CompassRose{
public:
    CompassRose();
    void render(const glm::mat4& MVP)const;

private:

    unsigned int vao = 0;
    unsigned int vertex_buffer = 0;
    unsigned int color_buffer = 0;

    unsigned int texture_x = 0;
    unsigned int texture_y = 0;
    unsigned int texture_z = 0;

    unsigned int simple_color_shader = 0;
    int simple_color_shader_mvp_id = 0;

    unsigned int textured_point_shader = 0;
    int textured_point_shader_mvp_id = 0;
    int textured_point_shader_image_id = 0;
};