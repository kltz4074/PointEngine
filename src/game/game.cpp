#include <iostream>
#include "../components/GameObject.cpp"
#include "../components/Mesh.cpp"
#include "../components/Camera.cpp"
#include "../engine.h"
#include "glm/fwd.hpp"

bool CanUp = false;


Mesh* cube = new Mesh;

void Start() {
    std::cout << "game started :>\n";
    for (auto obj : sceneObjects) obj->Start();
    Camera* camera = new Camera;

    camera->transform.position = glm::vec3(0, 0, 0);
    
    cube->material.texturePath = "resources/Textures/wall.jpg";
    cube->material.LoadTexture();
    cube->transform.position = glm::vec3(0, 0, 0);

    AddGameObject(cube);
    
}

void Update() {
    for (auto obj : sceneObjects) obj->Update();
    cube->transform.rotation.y += 1.0f * deltaTime;
    cube->transform.rotation.z += 1.0f * deltaTime;
}

void End() {
    RemoveObjs();
    sceneObjects.clear();
    std::cout << "game ended :<\n";
}