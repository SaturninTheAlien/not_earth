#version 330 core

out vec3 vFragColor;

uniform vec3 color;

void main(){
	vFragColor = color;
}