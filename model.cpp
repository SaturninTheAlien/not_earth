#include "model.h"
#include <stdexcept>
#include <GL/glew.h>
#include "model_loader.h"


Model::Model(const std::string& filename, bool normalize){
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> indexed_vertices; 
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;

    load_model(filename,
    indices,
    indexed_vertices,
    indexed_uvs,
    indexed_normals);

    if(normalize){
        normalize_model(indexed_vertices);
    }

    createBuffers(indices, indexed_vertices, indexed_uvs, indexed_normals);
}

void Model::createBuffers(
    const std::vector<unsigned short>& indices,
    const std::vector<glm::vec3>& indexed_vertices, 
    const std::vector<glm::vec2>& indexed_uvs,
    const std::vector<glm::vec3>& indexed_normals){
    //
    if(indexed_vertices.size()==0 || indices.size() == 0){
        throw std::runtime_error("Cannot create empty 3d model!");
    }

    this->hasPositions = true;
    this->hasNormals = indexed_normals.size() > 0;
    this->hasUVs = indexed_uvs.size() > 0;

    this->elements_number = indices.size();

    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &this->vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    if(this->hasUVs){
        glGenBuffers(1, &this->uv_buffer);
	    glBindBuffer(GL_ARRAY_BUFFER, this->uv_buffer);
	    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);
    }

    if(this->hasNormals){
        glGenBuffers(1, &this->normal_buffer);
	    glBindBuffer(GL_ARRAY_BUFFER, this->normal_buffer);
	    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
    }

    glGenBuffers(1, &this->elements_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elements_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
    glVertexAttribPointer(
        0,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    if(this->hasUVs){
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, this->uv_buffer);
        glVertexAttribPointer(
            1,                  // attribute
            2,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
    }

    if(this->hasNormals){
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, this->normal_buffer);
        glVertexAttribPointer(
            2,                  // attribute
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
    }


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elements_buffer);
    glBindVertexArray(0);   
}

void Model::draw()const{
    glBindVertexArray(this->VAO);
    glDrawElements(
        GL_TRIANGLES,      // mode
        this->elements_number,    // count
        GL_UNSIGNED_SHORT,   // type
        (void*)0           // element array buffer offset
    );
    glBindVertexArray(0);
}

Sphere::Sphere(int n){
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> indexed_vertices; 
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;

    createSphere(indices, indexed_vertices, indexed_uvs, n);
    this->createBuffers(indices, indexed_vertices, indexed_uvs, indexed_normals);
}