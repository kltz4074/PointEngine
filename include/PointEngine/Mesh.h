#ifndef POINTENGINE_MESH_H
#define POINTENGINE_MESH_H

#include "GameObject.h"
#include <string>

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

#endif // POINTENGINE_MESH_H

