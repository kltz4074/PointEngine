#include "Mesh.h"
#include "GameObject.h"
#include <iostream>

namespace PointEngine {

void Material::LoadTexture() {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Настройка фильтров и повторения
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = GL_RGB;
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture: " << texturePath << std::endl;
    }
    stbi_image_free(data);
}

void Mesh::Draw(GLuint shaderID) {
    if (model.ModelLoaded) {
        glBindVertexArray(model.VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material.textureID);

        glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    } else {
        model.loadOBJ("resources/Models/DefaultModel/Cube.obj");
        model.setupMesh();
        glBindVertexArray(model.VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material.textureID);

        glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        std::cout << "Model was loaded!" << std::endl;
    }
}
} // namespace PointEngine
