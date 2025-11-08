#include "game.h"
#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>

using namespace PointEngine;

// Game state
namespace {
    bool firstMouse = true;
    float lastX = 400.0f;
    float lastY = 300.0f;
    float yaw = -90.0f; // чтобы смотреть вперёд, а не вправо
    float pitch = 0.0f;
    const float sensitivity = 0.1f;
    bool CursorEnabled = false;
    glm::vec3 forward;
    
    Mesh* cube = nullptr;
    Mesh* cube2 = nullptr;
    PointLight* pointLight = nullptr;
    Camera* userCamera = nullptr;
    std::string wallTexture = "resources/Textures/container.jpg";
}

namespace PointEngine {
    void Start() {
        std::cout << "game started :>\n";
        
        // Initialize game objects
        cube = new Mesh;
        cube2 = new Mesh;
        pointLight = new PointLight({1.0f, 1.0f, 1.0f}, 1.0f);
        userCamera = new Camera;
        
        userCamera->transform.position = {0, 0, 3};
        userCamera->transform.rotation = {0, 0, 0};
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

        forward = glm::vec3(0.0f, 0.0f, -1.0f);
        
        // Call Start on all game objects
        for (auto obj : GetSceneObjects()) obj->Start();
    }

    void Update() {
        // Access scene objects through the accessor function
        for (auto obj : GetSceneObjects()) obj->Update();
        if (cube) {
            cube->transform.rotation.y += 1.0f * GetDeltaTime();
        }
    }

    void End() {
        RemoveObjs();
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

        yaw += xoffset;
        pitch += yoffset;

        // ограничиваем pitch, чтобы камера не переворачивалась
        if (pitch > 89.0f)  pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        forward.y = sin(glm::radians(pitch));
        forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        forward = glm::normalize(forward);

        // обновляем направление камеры
        if (userCamera) {
            userCamera->transform.rotation.y = yaw;
            userCamera->transform.rotation.x = pitch;
        }
    }

    void ProcessInput(GLFWwindow* window) {
        if (!userCamera) return;
        
        float BasicSpeed = 5.0f * GetDeltaTime();
        float RunningSpeed = 10.0f * GetDeltaTime();
        float currentSpeed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? RunningSpeed : BasicSpeed;

        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0, 1.0, 0.0)));
        glm::vec3 up = glm::normalize(glm::cross(right, forward));

        // движение камеры
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            userCamera->transform.position += forward * currentSpeed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            userCamera->transform.position -= forward * currentSpeed;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            userCamera->transform.position -= right * currentSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            userCamera->transform.position += right * currentSpeed;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            userCamera->transform.position.y -= currentSpeed;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            userCamera->transform.position.y += currentSpeed;
            
        static bool escPressedLastFrame = false;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            if (!escPressedLastFrame) { // сработает только один раз на нажатие
                CursorEnabled = !CursorEnabled;
                glfwSetInputMode(window, GLFW_CURSOR, 
                    CursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
            }
            escPressedLastFrame = true;
        } else {
            escPressedLastFrame = false;
        }
    }
    
    Camera* GetUserCamera() {
        return userCamera;
    }
    
    PointLight* GetPointLight() {
        return pointLight;
    }
}
