#include "texture.h"

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <GL/glew.h>

namespace py = pybind11;

unsigned int load_texture(const std::string& filename){

    //py::scoped_interpreter guard{};

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
    //unsigned int depth = np_array.shape(2);

    unsigned int TextureID = 0;
    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_2D, TextureID);   
   
    glTexImage2D(GL_TEXTURE_2D, 0, mode_gl, width, height, 0, mode_gl, GL_UNSIGNED_BYTE, np_array.data());
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return TextureID;
}