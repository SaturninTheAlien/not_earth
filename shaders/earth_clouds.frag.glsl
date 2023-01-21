#version 330 core
out vec3 vFragColor;

uniform sampler2D image;
//uniform sampler2D image_normals;
uniform sampler2D image_spec;
uniform sampler2D image_night;

in vec3 fNormal;
in vec3 fLightDir;
in vec2 fUV;

void main() {

	//vec3 normal1 = fNormal + texture( image_normals, fUV).rgb;

    vec3 normal1 = fNormal;

	vec3 diffuseColor = texture( image, fUV ).rgb;
    vec3 ambientColor = texture( image_night, fUV).rgb;

	float diff=max(0.0, dot(normalize(normal1),normalize(fLightDir)));

    vec3 color =diff * diffuseColor;
    if(diff < 0.25){
        color += (1 - 4*diff)*ambientColor;
    }
    

    if(texture( image_spec, fUV ).r >= 0.5){
        vec3 vReflection = normalize(reflect(-fLightDir,normal1));
        float spec = max(0.0f, dot(normalize(normal1),vReflection));
        if(diff!=0)
        {
            float fSpec = pow(spec, 128.0f);
            color += vec3(fSpec,fSpec,fSpec);
        }
    }

	vFragColor.r=min(1.0f,color.r);
	vFragColor.g=min(1.0f,color.g);
	vFragColor.b=min(1.0f,color.b);
}