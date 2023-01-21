#version 330 core

layout(location = 0) in vec3 vertex_position;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 lightPosition;


out vec3 fNormal;
out vec3 fLightDir;

void main() {
    vec4 v = V * M * vec4(vertex_position,1);
	

    fNormal = mat3(transpose(inverse(Model))) * vertex_position; //vNormal

    vec3 v3 = v.xyz / v.w;
    fLightDir = normalize(lightPosition - v3);
    
    gl_Position = P * v;
}