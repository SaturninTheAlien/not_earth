#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

void computeMatricesFromInputs(GLFWwindow* window, glm::vec3&position, glm::mat4& P, glm::mat4& V, float speed = 3.f, float mouseSpeed = 0.005f);
