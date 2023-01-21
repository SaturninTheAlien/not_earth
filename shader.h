#pragma once

#include <string>
unsigned int compileShader(const std::string& vs, const std::string & fs);
int getUniformID(int shader, const std::string& name);