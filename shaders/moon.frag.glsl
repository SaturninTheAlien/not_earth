#version 330 core
out vec3 vFragColor;

uniform sampler2D image;

in vec3 fNormal;
in vec3 fLightDir;
in vec2 fUV;

void main() {
	vec3 diffuseColor = texture( image, fUV ).rgb;

	float diff=max(0.0, dot(normalize(fNormal),normalize(fLightDir)));
	vec3 color = diff * diffuseColor;	

	vFragColor.r=min(1.0f,color.r);
	vFragColor.g=min(1.0f,color.g);
	vFragColor.b=min(1.0f,color.b);
}