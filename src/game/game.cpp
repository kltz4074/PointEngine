#include "game.h"
#include "engine.h"
#include "../components/LightManager.h"
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>

using namespace PointEngine;

// Game state
namespace {
    const float sensitivity = 0.1f;
    bool CursorEnabled = true;
    glm::vec3 forward;
    glm::vec2 MousePos;

    Mesh* cube = nullptr;
    Mesh* cube2 = nullptr;
    Camera* userCamera = nullptr;

    std::string wallTexture = "resources/Textures/container.jpg";
    std::string brickTexture = "resources/Textures/wall.jpg";
}

namespace PointEngine {

    void Start() { // on programm starts
        std::cout << "game started :>\n";

        // === Камера ===
        userCamera = new Camera;
        userCamera->transform.position = { 0, 0, 3 };
        userCamera->transform.rotation = { 0, 0, 0 };

        auto* pointLight = new PointLight(glm::vec3(1.0f), 1.0f);
        auto* pointLight2 = new PointLight(glm::vec3(1.0f), 2000.0f);
        

        pointLight->transform.position = { 0.0f, 1.0f, 2.0f };
        pointLight->transform.position = { 0.0f, 1.0f, 0.0f };
        AddPointLight(pointLight);

        auto* dirLight = new DirectionalLight(
            glm::vec3(-0.2f, -1.0f, -0.3f),
            glm::vec3(0.2f),
            glm::vec3(0.5f),
            glm::vec3(1.0f)
        );
        AddPointLight(pointLight2);
        // === Объекты ===
        cube = new Mesh;
        cube->transform.position = { 1, 0, 0 };
        cube->material.texturePath = wallTexture;
        cube->material.LoadTexture();
        AddGameObject(cube);

        cube2 = new Mesh;
        cube2->transform.position = { 0, -2, 0 };
        cube2->transform.scale = { 20, 1, 20 };
        cube2->material.texturePath = brickTexture;
        cube2->material.LoadTexture();
        AddGameObject(cube2);

        forward = glm::vec3(0.0f, 0.0f, -1.0f);

        // Вызов Start() у всех объектов
        for (auto obj : GetSceneObjects()) obj->Start();
    }

    void Update() { // every frame
        forward = userCamera->GetForwardVector();

        for (auto obj : GetSceneObjects()) obj->Update();

        if (cube) {
            cube->transform.rotation.y += 90.0f * GetDeltaTime();
        }
        
        GLFWwindow* window = PointEngine::GetGlfwWindow();

        userCamera->FlyCameraMovement(window, MousePos.x, MousePos.y, sensitivity, userCamera, forward, 5.0f, 9.0f, CursorEnabled);
    }

    void End() { // when programm closes
        RemoveObjs();
        std::cout << "game ended :<\n";
    }

    void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
        MousePos = {xpos, ypos};
    }

    glm::vec2 GetMousePos() {
        return MousePos;
    }
    Camera* GetUserCamera() {
        return userCamera;
    }
}
