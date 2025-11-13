#include "../core/Shader.h"
#include <glm/glm.hpp>

class Material {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    Shader* shader;

    Material()
        : ambient(1.0f), diffuse(1.0f), specular(1.0f),
          shininess(32.0f), shader(nullptr) {}
};
