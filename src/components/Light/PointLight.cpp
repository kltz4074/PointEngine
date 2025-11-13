#include "PointLight.h"

namespace PointEngine {
    PointLight::PointLight(
        glm::vec3 col, 
        float intens,
        float constant,
        float linear,
        float quadratic,
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular)
        
        : 
            color(col), 
            intensity(intens), 
            constant(constant), 
            linear(linear), 
            quadratic(quadratic),
            ambient(ambient), 
            diffuse(diffuse), 
            specular(specular) {}
}