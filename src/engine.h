#ifndef ENGINE_H
#define ENGINE_H

#include "glm/fwd.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include "core/stb_image.h"
#include <glad/glad.h>

namespace PointEngine {

// Forward declaration or include the Texture struct definition
struct Texture {
    unsigned int id = 0;
    std::string type;
    std::string path;
};

/**
 * @brief utility function to read a file into a string
 * @param filePath path to the file to read
 * @return contents of the file as a string, or empty string on error
 */
inline std::string ReadFileToString(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::in | std::ios::binary);
    if (!file) {
        std::cerr << "Could not open the file - '" << filePath << "'\n";
        return {};
    }

    std::ostringstream contents;
    contents << file.rdbuf();
    return contents.str();
}

inline glm::vec3 convertColor255To1(const glm::vec3& color) {
    return glm::vec3(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
}

inline glm::vec3 convertColor1To255(const glm::vec3& color) {
    return glm::vec3(color.r * 255.0f, color.g * 255.0f, color.b * 255.0f);
}

Texture LoadGLTexture(const std::string& path, const std::string& type)
{
    Texture tex{};
    tex.path = path;
    tex.type = type;

    // ---- load image -------------------------------------------------
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        tex.id = 0;
        return tex;
    }

    // ---- determine format -------------------------------------------
    GLenum internalFormat = 0, dataFormat = 0;
    if (nrChannels == 1) { internalFormat = GL_RED;  dataFormat = GL_RED;  }
    if (nrChannels == 3) { internalFormat = GL_RGB;  dataFormat = GL_RGB;  }
    if (nrChannels == 4) { internalFormat = GL_RGBA; dataFormat = GL_RGBA; }

    // ---- generate OpenGL texture ------------------------------------
    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height,
                 0, dataFormat, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    // ---- default parameters (feel free to tweak) --------------------
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return tex;
}

/**
 * @brief get the current delta time (time since last frame)
 * @return delta time in seconds
 */
double GetDeltaTime();

/**
 * @brief set the delta time (internal use only (as i understand))
 * @param dt delta time in seconds
 */
void SetDeltaTime(double dt);

} // namespace PointEngine

#endif // ENGINE_H