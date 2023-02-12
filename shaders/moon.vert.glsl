#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;

uniform mat4 MVP;
uniform mat4 N;

uniform vec3 lightPosition;

out vec3 fNormal;
out vec3 fLightDir;

out vec2 fUV;

void main() {
    
    vec4 v = N * vec4(vertex_position,1);
    
    fNormal = mat3(transpose(inverse(N))) * vertex_position; //vertex_normal

    vec3 v3 = v.xyz / v.w;
    fLightDir = normalize(lightPosition - v3);

    gl_Position = MVP * vec4(vertex_position,1);
    fUV = vertex_uv;
}