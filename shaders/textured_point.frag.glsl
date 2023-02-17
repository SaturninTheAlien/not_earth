#version 330 core

uniform sampler2D image;
out vec3 vFragColor;
in vec3 fColor;

void main(){

	vec4 color = texture(image, gl_PointCoord);
	if(color.a < 0.1)discard;

	vFragColor = color.rgb * fColor;
}