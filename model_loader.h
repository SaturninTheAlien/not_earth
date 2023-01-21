#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

void load_model(const std::string& filename,
    std::vector<unsigned short> & indices,
    std::vector<glm::vec3> & vertices, 
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals);

void normalize_model(std::vector<glm::vec3>& vertices);

void createSphere(std::vector<unsigned short> & indices,
    std::vector<glm::vec3>&vertices,
    std::vector<glm::vec2>&UVs,int n=40);
