#include "game.h"
#include "GLFW/glfw3.h"
#include "components/GameObject.h"
#include "engine.h"
#include "../components/LightManager.h"
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include "../components/Model.h"
using namespace PointEngine;

// Game state
namespace {
    bool firstMouse = true;
    float lastX = 400.0f;
    float lastY = 300.0f;
    float yaw = -90.0f; // чтобы смотреть вперёд
    float pitch = 0.0f;
    const float sensitivity = 0.1f;
    bool CursorEnabled = true;
    glm::vec3 forward;

    Mesh* pickle = nullptr;
    Mesh* plane = nullptr;
    Mesh* city = nullptr;
    Mesh* SunDirModel = nullptr;
    Mesh* mikuPlush = nullptr;
    Camera* userCamera = nullptr;

    std::string wallTexture = "resources/Textures/container.jpg";
    std::string brickTexture = "resources/Textures/wall.jpg";

    bool FullScreen = false;
    int windowPosX, windowPosY;
    int windowWidth = 800, windowHeight = 600;
}

namespace PointEngine {

    void Start() {
        std::cout << "game started :>\n";

        // === Камера ===
        userCamera = new Camera;
        userCamera->transform.position = { 0, 0, 3 };
        userCamera->transform.rotation = { 0, 0, 0 };

        //auto* pointLight = new PointLight(glm::vec3(1.0f), 1.0f);
        auto* pointLight2 = new PointLight(glm::vec3(1.0f), 1.0f);
        
        //pointLight->transform.position = { 0.0f, 1.0f, 2.0f };
        pointLight2->transform.position = { 0.0f, 1.0f, 0.0f };
        //AddPointLight(pointLight);

        auto* dirLight = new DirectionalLight(
            glm::vec3(0.01f),
            glm::vec3(0.01f),
            glm::vec3(0.1f)
        );
        dirLight->transform.rotation = {122.673f, -43.8395f, -71.415f};
        dirLight->transform.position.y -= 20;
        AddDirectionalLight(dirLight);
        AddPointLight(pointLight2);

        pickle = new Mesh;
        pickle->transform.position = { 1, 0.9, 0 };
        pickle->transform.scale *= 0.02;
        pickle->material.shininess = 10;
        pickle->material.LoadTexture("resources/Textures/cucumber.jpg");
        pickle->model.loadOBJ("resources/Models/DefaultModel/Pickle.obj");
        pickle->model.setupMesh();
        AddGameObject(pickle);

        plane = new Mesh;
        plane->transform.position = { 0, -1, 0 };
        plane->transform.scale = { 20, 0.1, 20 };
        plane->material.LoadTexture(brickTexture);
        AddGameObject(plane);

        city = new Mesh;
        city->transform.position = { 1, 0.2, 0 };
        city->transform.scale *= 0.2;
        city->material.LoadTexture("resources/Models/xdlol/texture4k.png");
        city->model.loadOBJ("resources/Models/xdlol/uwu.obj");
        city->model.setupMesh();
        AddGameObject(city); 

        mikuPlush = new Mesh;
        mikuPlush->transform.position = {0, 3, 0};
        mikuPlush->material.LoadTexture("resources/Models/miku/miku.png");
        mikuPlush->model.loadOBJ("resources/Models/miku/miku.obj");
        mikuPlush->model.setupMesh();
        AddGameObject(mikuPlush); 
        
        forward = glm::vec3(0.0f, 0.0f, -1.0f);

        for (auto obj : GetSceneObjects()) obj->Start();
    }

    void Update() {
        forward = userCamera->GetForwardVector();

        for (auto obj : GetSceneObjects()) obj->Update();

        pickle->transform.rotation.y += 0.5f * GetDeltaTime();
        mikuPlush->transform.rotation.y += 0.5f * GetDeltaTime();
    }

    void End() {
        RemoveObjs();
        std::cout << "game ended :<" << std::endl;
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
        float yoffset = lastY - y;
        lastX = x;
        lastY = y;

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        forward.y = sin(glm::radians(pitch));
        forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        forward = glm::normalize(forward);

        if (userCamera) {
            userCamera->transform.rotation.y = yaw;
            userCamera->transform.rotation.x = pitch;
        }
    }

    void ProcessInput(GLFWwindow* window) {
        if (!userCamera) return;

        float basicSpeed = 5.0f * GetDeltaTime();
        float runSpeed = 10.0f * GetDeltaTime();
        float speed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? runSpeed : basicSpeed;
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0, 1.0, 0.0)));
        glm::vec3 up = glm::normalize(glm::cross(right, forward));


        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            userCamera->transform.position += forward * speed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            userCamera->transform.position -= forward * speed;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            userCamera->transform.position -= right * speed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            userCamera->transform.position += right * speed;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            userCamera->transform.position.y -= speed;

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            userCamera->transform.position.y += speed;
    }
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    
        if (action == GLFW_PRESS && key == GLFW_KEY_F11) {
            FullScreen = !FullScreen;

            if (FullScreen) {
                // Сохранение позиции и размера окна перед фуллскрином
                glfwGetWindowPos(window, &windowPosX, &windowPosY);
                glfwGetWindowSize(window, &windowWidth, &windowHeight);

                const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

                glfwSetWindowMonitor(
                    window,
                    glfwGetPrimaryMonitor(),
                    0, 0,
                    mode->width, mode->height,
                    mode->refreshRate
                );

                std::cout << "fullscreen enabled\n";
            }
            else {
                // Возврат окна в обычный режим
                glfwSetWindowMonitor(
                    window,
                    nullptr,
                    windowPosX, windowPosY,     // возвращаем старую позицию
                    windowWidth, windowHeight, // возвращаем старый размер
                    0
                );

                std::cout << "windowed mode restored\n";
            }
        }


        static bool escPressedLastFrame = false;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            if (!escPressedLastFrame) {
                CursorEnabled = !CursorEnabled;
                glfwSetInputMode(window, GLFW_CURSOR,
                    CursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
            }
            firstMouse = true;
            escPressedLastFrame = true;
        }
        else {
            escPressedLastFrame = false;
        }
    }

    Camera* GetUserCamera() {
        return userCamera;
    }
}
