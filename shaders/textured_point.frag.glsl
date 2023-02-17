#version 330 core

uniform sampler2D image;
out vec3 color;

void main(){

	vec4 color4 = texture(image, gl_PointCoord);
	//if(color4.a < 0.1)discard;
	color = color4.rgb;
}