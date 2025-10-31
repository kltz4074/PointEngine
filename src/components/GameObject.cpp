#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <vector>
#include <string>


struct Transform {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    Transform() : position(0.0f), rotation(0.0f), scale(1.0f) {}

    glm::mat4 GetMatrix() const {
        glm::mat4 model(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1,0,0));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0,1,0));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0,0,1));

        model = glm::scale(model, scale);
        return model;
    }

};

class GameObject {
public:
    std::string name;
    Transform transform;

    unsigned int VAO = 0;      // каждый объект может иметь свой VAO
    unsigned int vertexCount = 36; // default для куба

    GameObject(const std::string& name = "GameObject") : name(name) {}
    virtual ~GameObject() {}

    virtual void Start() {}
    virtual void Update() {}
    virtual void Draw(unsigned int shaderProgram) {
        if(VAO == 0) return;

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glm::mat4 model = transform.GetMatrix();
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }
};

// === Глобальная сцена ===
std::vector<GameObject*> sceneObjects;

void AddGameObject(GameObject* obj) {
    sceneObjects.push_back(obj);
}

void UpdateAll() {
    for(auto obj : sceneObjects)
        obj->Update();
}

void DrawAll(unsigned int shaderProgram) {
    for(auto obj : sceneObjects)
        obj->Draw(shaderProgram);
}


