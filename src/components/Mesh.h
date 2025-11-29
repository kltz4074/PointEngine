#ifndef MESH_H
#define MESH_H

#include "GameObject.h"
#include <string>
#include <glad/glad.h>
#include "../core/stb_image.h"
#include "../core/shader.h"
#include "Model.h"
namespace PointEngine {

struct Material {
    std::string texturePath;
    GLuint DiffuseTextureID = 0;
    std::vector<GLuint> textures;
    float shininess = 32.0f;

    void LoadTexture(); // теперь создаёт textureID
};

class Mesh : public GameObject {
public:
    Material material;
    Model model;

    void Draw(Shader shader); // каждый Mesh умеет рисовать себя
};

} // namespace PointEngine

#endif // MESH_H
