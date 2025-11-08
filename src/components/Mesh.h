#ifndef MESH_H
#define MESH_H

#include "GameObject.h"
#include <string>
#include <iostream>
#include <glad/glad.h>
#include "../core/stb_image.h"

namespace PointEngine {

struct Material {
    std::string texturePath;
    float shininess;
    
    void LoadTexture() {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }
};

class Mesh : public GameObject {
public:
    Material material;
};

} // namespace PointEngine

#endif // MESH_H

