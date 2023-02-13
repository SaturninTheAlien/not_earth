#version 330 core
out vec3 vFragColor;

uniform sampler2D image;
uniform sampler2D image_normal;

in vec3 fTangentLightPos;
in vec3 fTangentFragPos;
in vec2 fUV;

void main() {

    vec3 normal = texture( image_normal, fUV ).rgb;
    normal = normalize(normal * 2.0 - 1.0);

	vec3 diffuseColor = texture( image, fUV ).rgb;

	vec3 lightDir = normalize(fTangentLightPos - fTangentFragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 color = diff * diffuseColor;

	vFragColor.r=min(1.0f,color.r);
	vFragColor.g=min(1.0f,color.g);
	vFragColor.b=min(1.0f,color.b);
}