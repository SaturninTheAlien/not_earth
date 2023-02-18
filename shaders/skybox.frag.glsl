#version 330 core

in vec3 fTextCoord;
out vec3 color;

uniform samplerCube cubemap;

void main(){
    color = texture(cubemap, fTextCoord).rgb;
}