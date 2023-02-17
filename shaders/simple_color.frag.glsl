#version 330 core

out vec3 vFragColor;
in vec3 fColor;

void main(){
	vFragColor = fColor;
}