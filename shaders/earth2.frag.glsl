#version 330 core
out vec3 vFragColor;

uniform sampler2D image;
uniform sampler2D image_normal;
uniform sampler2D image_spec;
uniform sampler2D image_night;

in vec3 fTangentLightPos;
in vec3 fTangentFragPos;
in vec2 fUV;

void main() {
    vec3 normal = texture( image_normal, fUV ).rgb;
    normal = normalize(normal * 2.0 - 1.0);

	vec3 diffuseColor = texture( image, fUV ).rgb;
    vec3 ambientColor = texture( image_night, fUV).rgb;

    vec3 lightDir = normalize(fTangentLightPos - fTangentFragPos);
	float diff = max(dot(lightDir, normal), 0.0);

    vec3 color =diff * diffuseColor;
    if(diff < 0.25){
        color += (1 - 4*diff)*ambientColor;
    }
    

    if(texture( image_spec, fUV ).r >= 0.5){
        vec3 vReflection = normalize(reflect(lightDir,normal));
        float spec = max(0.0f, dot(normalize(normal),vReflection));
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