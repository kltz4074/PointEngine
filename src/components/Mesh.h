#ifndef MESH_H
#define MESH_H

#include "GameObject.h"
#include <string>
#include <glad/glad.h>
#include "../core/stb_image.h"

namespace PointEngine {

struct Material {
    std::string texturePath;
    GLuint textureID = 0; // ID текстуры для OpenGL
    float shininess = 32.0f;

    void LoadTexture(); // теперь создаёт textureID
};

class Mesh : public GameObject {
public:
    Material material;
    
    void Draw(GLuint shaderID, GLuint VAO); // каждый Mesh умеет рисовать себя
};

} // namespace PointEngine

#endif // MESH_H
