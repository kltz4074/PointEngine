#include <iostream>
#include "../components/GameObject.cpp"
void Start() {
    std::cout << "game started :>\n";
    for (auto obj : sceneObjects) obj->Start();
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
