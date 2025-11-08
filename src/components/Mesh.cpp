#include "Mesh.h"
#include <iostream>
#include "../core/shader.h"
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

    void ApplyShader(Shader* shader) {
        shader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, MaterialShader->ID);
        shader->setVec3("material.ambient", ambient);
        shader->setVec3("material.diffuse", diffuse);
        shader->setVec3("material.specular", specular);
        shader->setFloat("material.shininess", shininess);
    }
public:
    std::string texturePath;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    Shader* MaterialShader;
    float shininess;
};

} // namespace PointEngine
