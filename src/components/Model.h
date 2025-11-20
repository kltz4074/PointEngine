#ifndef MODEL_H
#define MODEL_H

#include <cstddef>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "../core/stb_image.h"
#include <rapidobj/rapidobj.hpp>
#include <filesystem>
#include "iostream"
class Model {

public:
    std::string ModelPath;
    std::vector<double> vertices;
    std::vector<double> indices;

        void something(std::string ObjPath) {
            auto result = rapidobj::ParseFile(ObjPath);

            rapidobj::Triangulate(result);
            auto num_triangles = size_t();

            for (const auto& shape : result.shapes) {
                num_triangles += shape.mesh.num_face_vertices.size();
            }

            std::cout << "Shapes:    " << result.shapes.size() << '\n';
            std::cout << "Triangles: " << num_triangles << '\n';
            std::cout << "vertices: " << num_triangles * 3 << "\n";
    };
};

#endif