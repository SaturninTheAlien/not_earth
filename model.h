#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

class Model{
public:
    Model(const std::string& filename, bool isKsienrzyc=false);
    Model(const Model& src) = delete;

    Model(const std::vector<unsigned short>& indices,
        const std::vector<glm::vec3>& indexed_vertices, 
        const std::vector<glm::vec2>& indexed_uvs,
        const std::vector<glm::vec3>& indexed_normals){
            createBuffers(indices, indexed_vertices, indexed_uvs, indexed_normals);
        }

    Model & operator=(const Model& src) = delete;
    
    virtual ~Model(){

    }
    void draw()const;
protected:
    Model() = default;

    void createBuffers(
        const std::vector<unsigned short>& indices,
        const std::vector<glm::vec3>& indexed_vertices, 
        const std::vector<glm::vec2>& indexed_uvs,
        const std::vector<glm::vec3>& indexed_normals);

    bool hasPositions = false;
    bool hasUVs = false;
    bool hasNormals = false;

    unsigned int VAO = 0;
    unsigned int vertex_buffer = 0;
    unsigned int normal_buffer = 0;
    unsigned int uv_buffer = 0;
    unsigned int elements_buffer = 0;

    unsigned int elements_number = 0;
};

class Sphere: public Model{
public:
    Sphere(int n=40);

    Sphere(const Sphere& src) = delete;
    Sphere& operator= (const Sphere& src) = delete;
};