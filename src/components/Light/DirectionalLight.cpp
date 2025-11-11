// DirectionalLight.cpp
#include "DirectionalLight.h"

namespace PointEngine {

DirectionalLight::DirectionalLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float constAtt, float linAtt, float quadAtt)
    : direction(dir), ambient(amb), diffuse(diff), specular(spec), constant(constAtt), linear(linAtt), quadratic(quadAtt) {}
} // namespace PointEngine