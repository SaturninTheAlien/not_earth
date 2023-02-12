#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 N;

//uniform vec3 lightPosition;

out vec3 fNormal;
out vec3 fLightDir;

out vec2 fUV;

void main() {
    vec3 lightPosition = vec3(-100, 0, 0);

    vec4 v = V * N * vec4(vertex_position,1);
    fNormal = mat3(transpose(inverse(N))) * vertex_position;
    vec3 v3 = v.xyz / v.w;
    fLightDir = normalize(lightPosition - v3);

    /*vec4 v4 = lightPosition - v;
    fLightDir = normalize(v4.xyz / v4.w);*/

    gl_Position = P * V * M * vec4(vertex_position,1);
    fUV = vertex_uv;
}