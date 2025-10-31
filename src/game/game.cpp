#include <iostream>
#include "../components/GameObject.cpp"
#include "../engine.h"

bool CanUp = false;

// В game.cpp, после #include и других классов
class RotatingCube : public GameObject {
public:
    void Start() override {
        transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        transform.rotation = glm::vec3(0.0f);
        transform.scale = glm::vec3(1.0f);
    }

    void Update() override {
        float rotSpeed = 50.0f; // градусов в секунду
        transform.rotation.x += rotSpeed * deltaTime;
        transform.rotation.y += rotSpeed * deltaTime;
        transform.rotation.z += rotSpeed * deltaTime;
    }
};



void Start() {
    std::cout << "game started :>\n";
    AddGameObject(new RotatingCube());

    
}

void Update() {
    for (auto obj : sceneObjects) obj->Update();

}

void End() {
    for (auto obj : sceneObjects)
        delete obj;
    sceneObjects.clear();
    std::cout << "game ended :<\n";
}
