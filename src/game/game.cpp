// game.cpp
#include "game.h"
#include "engine.h"
#include "../components/LightManager.h"
#include "../components/model.h"

#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace PointEngine;

/* ------------------------------------------------------------- */
/* 1. Game State (только нужные переменные)                      */
/* ------------------------------------------------------------- */
namespace
{
    Camera* userCamera = nullptr;

    bool cursorEnabled = true;

    // Мышь
    bool firstMouse = true;
    float lastX = 0.0f, lastY = 0.0f;

    float yaw = -90.0f;
    float pitch = 0.0f;
    const float sensitivity = 0.12f;

    glm::vec3 forward(0,0,-1);

    // Модель
    Model* backpackModel = nullptr;
}

/* ------------------------------------------------------------- */
/* 2. Start()                                                    */
/* ------------------------------------------------------------- */
namespace PointEngine
{

void Start()
{
    std::cout << "Game started\n";

    /* ---------- Camera ---------- */
    userCamera = new Camera();
    userCamera->transform.position = glm::vec3(0,0,3);

    /* ---------- Lights ---------- */
    auto* pl1 = new PointLight(glm::vec3(1.0f), 1.0f);
    auto* pl2 = new PointLight(glm::vec3(1.0f), 1.0f);

    pl1->transform.position = glm::vec3(0,1,0);
    pl2->transform.position = glm::vec3(0,1,2);

    AddPointLight(pl1);
    AddPointLight(pl2);

    auto* dl = new DirectionalLight(
        glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.01f),
        glm::vec3(0.1f),
        glm::vec3(0.5f)
    );
    AddDirectionalLight(dl);

    /* ---------- Load Model ---------- */
    backpackModel = new Model("resources/Models/backpack/backpack.obj");

    /* ---------- Scene Objects Start() ---------- */
    for (auto* obj : GetSceneObjects())
        obj->Start();
}

/* ------------------------------------------------------------- */
/* 3. Update()                                                   */
/* ------------------------------------------------------------- */
void Update()
{
    glm::vec3 forward = userCamera->GetForwardVector();

    for (auto* obj : GetSceneObjects())
        obj->Update();
        
    Shader shader("./resources/shaders/shader.vs", "./resources/shaders/shader.fs");
    backpackModel->Draw(shader);
    /* ------- Draw our Model manually (упрощённо) ------- */
}

/* ------------------------------------------------------------- */
/* 4. End()                                                      */
/* ------------------------------------------------------------- */
void End()
{
    RemoveObjs();
    std::cout << "Game ended\n";
}

/* ------------------------------------------------------------- */
/* 5. Mouse Input                                                */
/* ------------------------------------------------------------- */
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!userCamera) return;

    float x = static_cast<float>(xpos);
    float y = static_cast<float>(ypos);

    if (firstMouse)
    {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    float xoffset = (x - lastX) * sensitivity;
    float yoffset = (lastY - y) * sensitivity;

    lastX = x;
    lastY = y;

    yaw   += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    userCamera->transform.rotation.x = pitch;
    userCamera->transform.rotation.y = yaw;
}

/* ------------------------------------------------------------- */
/* 6. Keyboard Input                                             */
/* ------------------------------------------------------------- */
void ProcessInput(GLFWwindow* window)
{
    if (!userCamera) return;

    float dt = GetDeltaTime();

    float speed =
        (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        ? 10.0f * dt
        :  5.0f * dt;

    glm::vec3 forward = userCamera->GetForwardVector();
    glm::vec3 right   = glm::normalize(glm::cross(forward, glm::vec3(0,1,0)));

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

    /* ---- ESC toggle cursor ---- */
    static bool lastEsc = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        if (!lastEsc)
        {
            cursorEnabled = !cursorEnabled;
            glfwSetInputMode(window, GLFW_CURSOR,
                cursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

            firstMouse = true;
        }
        lastEsc = true;
    }
    else lastEsc = false;
}

/* ------------------------------------------------------------- */
Camera* GetUserCamera()
{
    return userCamera;
}

} // namespace PointEngine