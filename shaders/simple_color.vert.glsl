#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;

uniform mat4 MVP;
out vec3 fColor;

void main(){
	gl_Position =  MVP * vec4(vertex_position,1);
	fColor = vertex_color;
}