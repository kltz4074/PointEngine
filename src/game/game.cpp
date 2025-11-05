#include <iostream>
#include <string>
#include "../components/GameObject.cpp"
#include "../components/Mesh.cpp"
#include "../components/camera.cpp"
#include "../engine.h"
#include "glm/fwd.hpp"
#include <GLFW/glfw3.h>

bool CanUp = false;


Mesh* cube = new Mesh;
Mesh* cube2 = new Mesh;
Mesh* cube3 = new Mesh;

Camera* UserCum = new Camera;
std::string wallTexture = "resources/Textures/wall.jpg";

void Start() {
    std::cout << "game started :>\n";
    for (auto obj : sceneObjects) obj->Start();
    UserCum->transform.position = {0, 0, 0};
    
    cube->transform.position = {1, 0, 0};
    cube->material.texturePath = wallTexture;
    
    cube2->transform.position = {0, -2, 0};
    cube2->material.texturePath = wallTexture;
    cube2->transform.scale.x = 20;
    cube2->transform.scale.z = 20;

    cube2->material.LoadTexture();
    cube->material.LoadTexture();
    AddGameObject(cube2);
    AddGameObject(cube);
}


void Update() {
    for (auto obj : sceneObjects) obj->Update();

    cube->transform.rotation += 1.0f * deltaTime;
}

void End() {
    RemoveObjs();
    sceneObjects.clear();
    std::cout << "game ended :<\n";
}

void ProcessInput(GLFWwindow* window) {
    float BasicSpeed = 5.0 * deltaTime;
    float RunningSpeed = 10.0f * deltaTime;
    float currentSpeed;
    float rotSpeed = 80.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        currentSpeed = RunningSpeed;
    } else {
        currentSpeed = BasicSpeed;
    }
    // --- вращение камеры ---
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        UserCum->transform.rotation.y -= rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        UserCum->transform.rotation.y += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        UserCum->transform.rotation.x += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        UserCum->transform.rotation.x -= rotSpeed;

    // --- вычисляем направление движения камеры ---
    glm::vec3 forward;
    forward.x = cos(glm::radians(UserCum->transform.rotation.y)) * cos(glm::radians(UserCum->transform.rotation.x));
    forward.y = sin(glm::radians(UserCum->transform.rotation.x));
    forward.z = sin(glm::radians(UserCum->transform.rotation.y)) * cos(glm::radians(UserCum->transform.rotation.x));
    forward = glm::normalize(forward);

    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0, 1.0, 0.0)));
    glm::vec3 up    = glm::normalize(glm::cross(right, forward));

    // --- движение камеры ---
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        UserCum->transform.position += forward * currentSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        UserCum->transform.position -= forward * currentSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        UserCum->transform.position -= right * currentSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        UserCum->transform.position += right * currentSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        UserCum->transform.position += up * currentSpeed;
    
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        UserCum->transform.position -= up * currentSpeed; 
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        UserCum->transform.position += up * currentSpeed;
}