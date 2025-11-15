#define STB_IMAGE_IMPLEMENTATION
#include "../core/stb_image.h"
#include "TextureLoader.h"
#include <glad/glad.h>
#include <iostream>

Texture LoadGLTexture(const std::string& path, const std::string& type)
{
    Texture tex{};
    tex.path = path;
    tex.type = type;

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return tex;
    }

    GLenum internalFormat = 0, dataFormat = 0;
    if (nrChannels == 1) { internalFormat = GL_RED;  dataFormat = GL_RED; }
    if (nrChannels == 3) { internalFormat = GL_RGB;  dataFormat = GL_RGB; }
    if (nrChannels == 4) { internalFormat = GL_RGBA; dataFormat = GL_RGBA; }

    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return tex;
}