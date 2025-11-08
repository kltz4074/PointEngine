#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <glm/glm.hpp>
#include "../GameObject.h"

namespace PointEngine {

class PointLight : public GameObject {
public:
    glm::vec3 color;
    float intensity;
    
    PointLight(glm::vec3 col, float intens)
        : color(col), intensity(intens) {}
};

} // namespace PointEngine

#endif // POINTLIGHT_H

