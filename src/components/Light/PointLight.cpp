#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../GameObject.cpp"

class PointLight : public GameObject{

public:
    glm::vec3 color;
    float intensity;
    
    PointLight(glm::vec3 col, float intens)
        : color(col), intensity(intens) {}

    
};