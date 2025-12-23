#ifndef MODEL_H
#define MODEL_H

#include <cstddef>
#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "../core/stb_image.h"
#include <rapidobj/rapidobj.hpp>
#include <filesystem>
#include <iostream>
#include <glad/glad.h>
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoord;
};
class Model {
public:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    GLuint VAO, VBO, EBO;
    bool ModelLoaded = false;

    GLuint DefaultVAO, DefaultVBO, DefaultEBO;

    inline void LoadModel(const std::string& ObjPath) {
        LoadModel(ObjPath);
        setupMesh();
    }

    void loadOBJ(const std::string& ObjPath);

    void setupMesh();
};

#endif
