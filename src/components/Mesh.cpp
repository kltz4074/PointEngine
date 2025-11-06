#include "GameObject.cpp"
#include <iostream>

struct Material {
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
public:
    std::string texturePath;
    
    float shininess;
};

class Mesh : public GameObject {
public:
    Material material;
};