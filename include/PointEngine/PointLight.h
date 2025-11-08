#ifndef POINTENGINE_POINTLIGHT_H
#define POINTENGINE_POINTLIGHT_H

#include <glm/glm.hpp>
#include "GameObject.h"

namespace PointEngine {

class PointLight : public GameObject {
public:
    glm::vec3 color;
    float intensity;
    
    PointLight(glm::vec3 col, float intens);
};

} // namespace PointEngine

#endif // POINTENGINE_POINTLIGHT_H

