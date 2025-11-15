#pragma once
#include <string>

struct Texture {
    unsigned int id = 0;
    std::string type;
    std::string path;
};

Texture LoadGLTexture(const std::string& path, const std::string& type = "texture_diffuse");