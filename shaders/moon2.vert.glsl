#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;

uniform mat4 MVP;
uniform mat4 N;

uniform vec3 lightPosition;

out vec3 fTangentLightPos;
out vec3 fTangentFragPos;
out vec2 fUV;


void main() {
    
    vec4 v = N * vec4(vertex_position,1);

    vec3 vertex_normal = vertex_position; //Sphere

    vec3 c1 = cross(vertex_normal, vec3(0.0, 0.0, 1.0));
    vec3 c2 = cross(vertex_normal, vec3(0.0, 1.0, 0.0));

    vec3 aTangent, aBitangent;

    if (length(c1)>length(c2)){
        aTangent = c1;
    }
    else{
        aTangent = c2;
    }
    aTangent = normalize(aTangent);
    aBitangent = cross(vertex_normal, aTangent);

    mat3 normalMatrix = transpose(inverse(mat3(N)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N_ = normalize(normalMatrix * vertex_normal);

    T = normalize(T - dot(T, N_) * N_);
    vec3 B = cross(N_, T);

    mat3 TBN = transpose(mat3(T, B, N_));

    fTangentLightPos = TBN * lightPosition;
    fTangentFragPos  = TBN * vec3(N * vec4(vertex_position, 1));

    gl_Position = MVP * vec4(vertex_position,1);
    fUV = vertex_uv;
}