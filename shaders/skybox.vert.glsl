#version 330 core

layout(location = 0) in vec3 vertex_position;

out vec3 fTextCoord;

uniform mat4 MVP;

void main(void){
    fTextCoord = vertex_position;
    vec4 pos = MVP * vec4(vertex_position, 1.0);
    pos.z = 0;
    gl_Position = pos;//pos.xyww;
}