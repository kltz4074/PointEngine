#include <glm/glm.hpp>
#include <string>
#include "Model.h"
#include "../core/stb_image.h"
#include <rapidobj/rapidobj.hpp>
#include <glad/glad.h>
#include <iostream>

void Model::loadOBJ(const std::string& ObjPath) {
    rapidobj::Result result = rapidobj::ParseFile(ObjPath);
    rapidobj::Triangulate(result);

    const auto& attrs = result.attributes;

    vertices.clear();
    indices.clear();

    for (const auto& shape : result.shapes) {
        for (const auto& index : shape.mesh.indices) {

            Vertex vert{};

            // --- position ---
            if (index.position_index >= 0) {
                vert.position = {
                    attrs.positions[index.position_index * 3 + 0],
                    attrs.positions[index.position_index * 3 + 1],
                    attrs.positions[index.position_index * 3 + 2]
                };
            }

            // --- normal ---
            if (!attrs.normals.empty() && index.normal_index != -1) {
                vert.normal = {
                    attrs.normals[index.normal_index * 3 + 0],
                    attrs.normals[index.normal_index * 3 + 1],
                    attrs.normals[index.normal_index * 3 + 2]
                };
            }

            // --- texcoord ---
            if (!attrs.texcoords.empty() && index.texcoord_index != -1) {
                vert.texcoord = {
                    attrs.texcoords[index.texcoord_index * 2 + 0],
                    1.0f - attrs.texcoords[index.texcoord_index * 2 + 1] // инверсия Y
                };
            }

            vertices.push_back(vert);
            indices.push_back(indices.size());
        }
    }

    std::cout << "Loaded verts: " << vertices.size() << "\n";
    ModelLoaded = true;
}

void Model::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // --- VBO ---
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                    vertices.size() * sizeof(Vertex),
                    vertices.data(),
                    GL_STATIC_DRAW);

    // --- EBO ---
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                    indices.size() * sizeof(uint32_t),
                    indices.data(),
                    GL_STATIC_DRAW);

    // --- layout positions ---
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                            sizeof(Vertex), (void*)offsetof(Vertex, position));

    // --- layout normals ---
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                            sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // --- layout uv ---
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                            sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

    glBindVertexArray(0);
}
