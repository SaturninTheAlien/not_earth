#pragma once

#include <string>

unsigned int compileShader(const std::string& vs, const std::string & fs);
int getUniformID(int shader, const std::string& name);

#include <glm/glm.hpp>

class SimpleTextureShader{
public:
    SimpleTextureShader();

    void use(glm::mat4 MVP, int texture_id)const;

    int getProgramID()const{
        return this->programID;
    }

    int programID = 0;
    int mvp_id = -1;
    int image_id = -1;
};