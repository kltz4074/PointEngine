#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include <glm/glm.hpp>
#include "../GameObject.h"

namespace PointEngine {

class DirectionalLight : public GameObject {
public:
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
    
    DirectionalLight(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float constAtt = 1.0f, float linAtt = 0.09f, float quadAtt = 0.032f);
};

} // namespace PointEngine

#endif // DIRECTIONALLIGHT_H

