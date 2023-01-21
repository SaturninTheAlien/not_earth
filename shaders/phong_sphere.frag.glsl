#version 330 core
out vec3 vFragColor;

uniform vec3 diffuseColor;
uniform vec3 ambientColor;

in vec3 fNormal;
in vec3 fLightDir;

void main() {

	float diff=max(0.0, dot(normalize(fNormal),normalize(fLightDir)));

	vec3 color =diff * diffuseColor;
	color +=ambientColor;
	
	vec3 vReflection = normalize(reflect(-fLightDir,fNormal));
	float spec = max(0.0f, dot(normalize(fNormal),vReflection));
	if(diff!=0)
	{
		float fSpec = pow(spec, 128.0f);
		color += vec3(fSpec,fSpec,fSpec);
	}
	vFragColor.r=min(1.0f,color.r);
	vFragColor.g=min(1.0f,color.g);
	vFragColor.b=min(1.0f,color.b);
}