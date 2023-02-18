#include "texture.h"

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <GL/glew.h>

namespace py = pybind11;

void textImage2D(GLenum target, const std::string& filename){
    py::module_ Image = py::module_::import("PIL.Image");
    auto pil_img = Image.attr("open")(filename);

    std::string mode_str = pil_img.attr("mode").cast<std::string>();
    int mode_gl = 0;

    if(mode_str == "RGBA"){
        mode_gl = GL_RGBA;
    }
    else if(mode_str == "RGB"){
        mode_gl = GL_RGB;
    }
    else{
        mode_gl = GL_RGB;
        pil_img = pil_img.attr("convert")("rgb");
    }

    py::array_t<unsigned char> np_array = pil_img;
    
    unsigned int height = np_array.shape(0);
    unsigned int width = np_array.shape(1);

    glTexImage2D(target, 0, mode_gl, width, height, 0, mode_gl, GL_UNSIGNED_BYTE, np_array.data());    
}

unsigned int load_texture(const std::string& filename){

    unsigned int TextureID = 0;
    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_2D, TextureID);   

    textImage2D(GL_TEXTURE_2D, filename);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0); 

    return TextureID;
}

unsigned int load_cubemap(const std::array<std::string, 6>& faces){

    unsigned int textureID = 0;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for(int i = 0; i < faces.size(); ++i){
        textImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, faces[i]);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;    
}