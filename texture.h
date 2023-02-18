#pragma once

#include <string>
#include <array>

unsigned int load_texture(const std::string& filename);
unsigned int load_cubemap(const std::array<std::string, 6>& faces);
