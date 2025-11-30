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
    GLuint DiffuseTextureID = 0;
    std::vector<GLuint> textures;
    float shininess = 32.0f;

    void LoadTexture(const std::string TexturePath);
};

class Mesh : public GameObject {
public:
    Material material;
    Model model;

    void Draw(Shader shader);
};

} // namespace PointEngine

#endif // MESH_H
