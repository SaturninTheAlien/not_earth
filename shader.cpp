#include "shader.h"

#include <string>
#include <sstream>
#include <GL/glew.h>


unsigned int compileShader(const std::string& vs, const std::string & fs){
    const char * vs_c = vs.c_str();
    const char * fs_c = fs.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vs_c, nullptr);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::ostringstream os;
        os  << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
        throw std::runtime_error(os.str());
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fs_c, nullptr);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::ostringstream os;
        os << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
        throw std::runtime_error(os.str());
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::ostringstream os;
        os << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        throw std::runtime_error(os.str());
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int getUniformID(int shader, const std::string& name){
    int uniform_id = glGetUniformLocation(shader, name.c_str());
    if(uniform_id<0){
        std::ostringstream os;
        os<<"Uniform variable \""<<name<<"\" not found! ["<<uniform_id<<"]";
        throw std::runtime_error(os.str());
    }

    return uniform_id;
}