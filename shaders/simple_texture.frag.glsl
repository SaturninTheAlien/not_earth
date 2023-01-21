#version 330 core

in vec2 fragment_uv;
out vec3 color;

uniform sampler2D image;

void main(){
    
	color = texture( image, fragment_uv ).rgb;
}