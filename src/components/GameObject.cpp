#include "GameObject.h"
#include <glad/glad.h>
#include <vector>


namespace PointEngine {

Transform::Transform()
    : position(0.0f), rotation(0.0f), scale(1.0f) {}

glm::mat4 Transform::GetMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
    model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
    model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));
    model = glm::scale(model, scale);
    return model;
}

std::vector<GameObject*> sceneObjects;

GameObject::GameObject(const std::string& name) : name(name) {}

GameObject::~GameObject() {}

void GameObject::Start() {}

void GameObject::Update() {}

void GameObject::Draw(unsigned int shaderProgram, unsigned int VAO) {
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glm::mat4 model = transform.GetMatrix();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void AddGameObject(GameObject* obj) {
    sceneObjects.push_back(obj);
}

void DrawAll(unsigned int shaderProgram, unsigned int VAO) {
    for (auto obj : sceneObjects)
        obj->Draw(shaderProgram, VAO);
}

// even more cool stuff broh
void RemoveObjs() {
    for (auto obj : sceneObjects)
        delete obj;
    sceneObjects.clear();
}

std::vector<GameObject*>& GetSceneObjects() {
    return sceneObjects;
}

} // namespace PointEngine
