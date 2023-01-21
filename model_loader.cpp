#include "model_loader.h"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <limits>
#include <iostream>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

void load_model(const std::string& filename,

    std::vector<unsigned short> & indices,
    std::vector<glm::vec3> & vertices, 
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals){
    //

    Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename, 0/*aiProcess_JoinIdenticalVertices | aiProcess_SortByPType*/);
	if( !scene) {
        std::ostringstream os;
        os<< "Assimp error: "<<importer.GetErrorString();
        throw std::runtime_error(os.str());
	}
	const aiMesh* mesh = scene->mMeshes[0]; // In this simple example code we always use the 1rst mesh (in OBJ files there is often only one anyway)

	// Fill vertices positions
	vertices.reserve(mesh->mNumVertices);
	for(unsigned int i=0; i<mesh->mNumVertices; i++){
		aiVector3D pos = mesh->mVertices[i];
		vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
	}


	// Fill vertices texture coordinates
	uvs.reserve(mesh->mNumVertices);
	for(unsigned int i=0; i<mesh->mNumVertices; i++){
		aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
		uvs.push_back(glm::vec2(UVW.x, 1 - UVW.y));
	}


	// Fill vertices normals
	if(mesh->HasNormals()){	
		normals.reserve(mesh->mNumVertices);
		for(unsigned int i=0; i<mesh->mNumVertices; i++){
			aiVector3D n = mesh->mNormals[i];
			normals.push_back(glm::vec3(n.x, n.y, n.z));
		}
	}

	// Fill face indices
	indices.reserve(3*mesh->mNumFaces);
	for (unsigned int i=0; i<mesh->mNumFaces; i++){
		// Assume the model has only triangles.
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
	}
	
	// The "scene" pointer will be deleted automatically by "importer"
}

void fix_ksienrzyc_model(std::vector<glm::vec3>& vertices){
	float min_x = std::numeric_limits<float>::max();
	float min_y = std::numeric_limits<float>::max();
	float min_z = std::numeric_limits<float>::max();

	float max_x = std::numeric_limits<float>::min();
	float max_y = std::numeric_limits<float>::min();
	float max_z = std::numeric_limits<float>::min();

	for(const glm::vec3& vertex:vertices){
		if(vertex.x < min_x){
			min_x = vertex.x;
		}
		if(vertex.x > max_x){
			max_x = vertex.x;
		}

		if(vertex.y < min_y){
			min_y = vertex.y;
		}
		if(vertex.y > max_y){
			max_y = vertex.y;
		}
		if(vertex.z < min_z){
			min_z = vertex.z;
		}
		if(vertex.z > max_z){
			max_z = vertex.z;
		}
	}

	float w = max_x - min_x;
	float h = max_y - min_y;
	float d = max_z - min_z;

	float s = w;
	if(s < h){
		s = h;
	}
	if(s < d){
		s = d;
	}

	/*std::cout<<"min_x = "<<min_x<<"\n"
	<<"max_x = "<<max_x<<"\n"

	<<"min_y = "<<min_y<<"\n"
	<<"max_y = "<<max_y<<"\n"

	<<"min_z = "<<min_z<<"\n"
	<<"max_z = "<<max_z<<std::endl;

	std::cout<<"s = "<<s<<std::endl;*/

	s/=2;

	for(glm::vec3 & vertex: vertices){

		vertex.z -= d/2;

		vertex /= s;

		float t = vertex.y;
		vertex.y = vertex.z;
		vertex.z = t;

		t = vertex.x;
		vertex.x = vertex.z;
		vertex.z = t;
	}
}


void createSphere(std::vector<unsigned short> & indices,
std::vector<glm::vec3>&vertices,
std::vector<glm::vec2>&UVs,int n){

	float x, y, z, xz;                              // vertex position
	float s, t;                                     // vertex texCoord

	float sectorStep = 2 * M_PI / n;
	float stackStep = M_PI / n;
	float sectorAngle, stackAngle;

	for(int i = 0; i <= n; ++i){

		stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xz = cosf(stackAngle);             // r * cos(u)
		y = sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for(int j = 0; j <= n; ++j){

			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = -xz * cosf(sectorAngle);             // r * cos(u) * cos(v)
			z = xz * sinf(sectorAngle);             // r * cos(u) * sin(v)

			vertices.push_back(glm::vec3(x,y,z));

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / n;
			t = (float)i / n;
			UVs.push_back(glm::vec2(s,t));
			/*UVs.push_back(s);
			UVs.push_back(t);*/
		}
	}

	unsigned int k1, k2;
    for(int i = 0; i < n; ++i)
    {
        k1 = i * (n + 1);     // beginning of current stack
        k2 = k1 + n + 1;      // beginning of next stack

        for(int j = 0; j < n; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding 1st and last stacks
            if(i != 0)
            {
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1+1);
                //addIndices(k1, k2, k1+1);   // k1---k2---k1+1
            }

            if(i != (n-1))
            {
				indices.push_back(k1+1);
				indices.push_back(k2);
				indices.push_back(k2+1);
                //addIndices(k1+1, k2, k2+1); // k1+1---k2---k2+1
            }

            // vertical lines for all stacks
            /*lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if(i != 0)  // horizontal lines except 1st stack
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }*/
        }
    }
}
