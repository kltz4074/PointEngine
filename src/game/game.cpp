#include <iostream>
#include "../components/GameObject.cpp"
#include "../engine.h"

bool CanUp = false;

class Cube : public GameObject {
public:
    float speed = 1.0f; // скорость движения (единица в секунду)
    float rotSpeed = 1.0f; // градусов в секунду
    
    void Start() override {
        transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        transform.rotation = glm::vec3(0.0f);
    }


    void Update() override {
        if (CanUp)
            transform.position.y += speed * deltaTime;
        else
            transform.position.y -= speed * deltaTime;

        if (transform.position.y >= 0.5f)
            CanUp = false;
        if (transform.position.y <= -0.5f)
            CanUp = true;

        transform.rotation.y += rotSpeed * deltaTime;
        transform.rotation.z += rotSpeed * deltaTime;
    }
};


void Start() {
    std::cout << "game started :>\n";
    for (auto obj : sceneObjects) obj->Start();
    AddGameObject(new Cube());
    
}

void Update() {
    for (auto obj : sceneObjects) obj->Update();

}

void End() {
    RemoveObjs();
    sceneObjects.clear();
    std::cout << "game ended :<\n";
}