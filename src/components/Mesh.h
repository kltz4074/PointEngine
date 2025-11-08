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
    
    void LoadTexture();
};

class Mesh : public GameObject {
public:
    Material material;
};

} // namespace PointEngine

#endif // MESH_H

