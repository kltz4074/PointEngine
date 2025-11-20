#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "../core/stb_image.h"
#include <rapidobj/rapidobj.hpp>
class Model {
    std::string ModelPath;
    std::vector<double> vertices;
    std::vector<double> indices;
    
    std::vector<double> GetVerticesFromOBJ {
        
    };
};

#endif