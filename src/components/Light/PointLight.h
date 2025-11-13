#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <glm/glm.hpp>
#include "../GameObject.h"
#include "glm/fwd.hpp"

namespace PointEngine {

class PointLight : public GameObject {
public:
    glm::vec3 color;
    float intensity;
    glm::vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    PointLight(
        glm::vec3 col, 
        float intens,
        float constant = 1.0f,
        float linear = 0.09f,
        float quadratic = 0.032f,
        glm::vec3 ambient = glm::vec3(0.2f),
        glm::vec3 diffuse = glm::vec3(0.8f),
        glm::vec3 specular = glm::vec3(1.0f)
    );
};

} // namespace PointEngine

#endif // POINTLIGHT_H

