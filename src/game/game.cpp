#include <iostream>
#include <string>
#include "../components/GameObject.cpp"
#include "../components/Mesh.cpp"
#include "../components/camera.cpp"
#include "../engine.h"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include <GLFW/glfw3.h>
#include "../components/Light/PointLight.cpp"

bool CanUp = false;
bool firstMouse = true;

Mesh* cube = new Mesh;
Mesh* cube2 = new Mesh;
PointLight* pointLight = new PointLight({1.0f, 1.0f, 1.0f}, 100.0f);
Camera* UserCum = new Camera;
std::string wallTexture = "resources/Textures/container.jpg";

float lastX = 400.0f;
float lastY = 300.0f;
float yaw   = -90.0f; // чтобы смотреть вперёд, а не вправо
float pitch = 0.0f;
const float sensitivity = 0.1f;
bool CursordEnabeled;
glm::vec3 forward;

void Start() {
    std::cout << "game started :>\n";
    for (auto obj : sceneObjects) obj->Start();

    
    UserCum->transform.position = {0, 0, 3};
    UserCum->transform.rotation = {0, 0, 0};
    pointLight->transform.position = {0, 0, 0};
    cube->transform.position = {1, 0, 0};
    cube->material.texturePath = wallTexture;

    cube2->transform.position = {0, -2, 0};
    cube2->transform.scale.x = 20;
    cube2->transform.scale.z = 20;
    cube2->material.texturePath = wallTexture;

    cube->material.LoadTexture();
    cube2->material.LoadTexture();

    AddGameObject(cube);
    AddGameObject(cube2);

    // захватываем курсор и скрываем его

    forward = glm::vec3(0.0f, 0.0f, -1.0f);
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


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float x = static_cast<float>(xpos);
    float y = static_cast<float>(ypos);

    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    float xoffset = x - lastX;
    float yoffset = lastY - y; // переворачиваем (как в OpenGL)
    lastX = x;
    lastY = y;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    // ограничиваем pitch, чтобы камера не переворачивалась
    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.y = sin(glm::radians(pitch));
    forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(forward);

    // обновляем направление камеры
    UserCum->transform.rotation.y = yaw;
    UserCum->transform.rotation.x = pitch;
}

void ProcessInput(GLFWwindow* window) {
    float BasicSpeed = 5.0f * deltaTime;
    float RunningSpeed = 10.0f * deltaTime;
    float currentSpeed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? RunningSpeed : BasicSpeed;

    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0, 1.0, 0.0)));
    glm::vec3 up    = glm::normalize(glm::cross(right, forward));

    // движение камеры
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        UserCum->transform.position += forward * currentSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        UserCum->transform.position -= forward * currentSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        UserCum->transform.position -= right * currentSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        UserCum->transform.position += right * currentSpeed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        UserCum->transform.position.y -= currentSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        UserCum->transform.position.y += currentSpeed;
    static bool escPressedLastFrame = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (!escPressedLastFrame) { // сработает только один раз на нажатие
            CursordEnabeled = !CursordEnabeled;
            glfwSetInputMode(window, GLFW_CURSOR, 
                CursordEnabeled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        }
        escPressedLastFrame = true;
    } else {
        escPressedLastFrame = false;
    }

}
