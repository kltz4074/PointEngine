// game.cpp
#include "game.h"
#include "engine.h"
#include "../components/LightManager.h"          // <-- our tiny loader (see below)
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace PointEngine;

/* --------------------------------------------------------------------- */
/* 1. Cube geometry – static, can be replaced by your own data later    */
/* --------------------------------------------------------------------- */
static std::vector<Vertex> CubeVertices()
{
    const float s = 0.5f;                     // half-size
    std::vector<Vertex> v(36);                // 6 faces × 6 verts (2 triangles)

    // Helper lambda to fill a face
    auto face = [&](int i, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,
                   glm::vec3 n, glm::vec2 uv0, glm::vec2 uv1,
                   glm::vec2 uv2, glm::vec2 uv3)
    {
        v[i*6+0] = {p0, n, uv0};
        v[i*6+1] = {p1, n, uv1};
        v[i*6+2] = {p2, n, uv2};
        v[i*6+3] = {p2, n, uv2};
        v[i*6+4] = {p3, n, uv3};
        v[i*6+5] = {p0, n, uv0};
    };

    // front
    face(0,
         {-s,-s, s}, { s,-s, s}, { s, s, s}, {-s, s, s},
         {0,0,1}, {0,0}, {1,0}, {1,1}, {0,1});
    // back
    face(1,
         { s,-s,-s}, {-s,-s,-s}, {-s, s,-s}, { s, s,-s},
         {0,0,-1}, {0,0}, {1,0}, {1,1}, {0,1});
    // left
    face(2,
         {-s,-s,-s}, {-s,-s, s}, {-s, s, s}, {-s, s,-s},
         {-1,0,0}, {0,0}, {1,0}, {1,1}, {0,1});
    // right
    face(3,
         { s,-s, s}, { s,-s,-s}, { s, s,-s}, { s, s, s},
         {1,0,0}, {0,0}, {1,0}, {1,1}, {0,1});
    // bottom
    face(4,
         {-s,-s,-s}, { s,-s,-s}, { s,-s, s}, {-s,-s, s},
         {0,-1,0}, {0,0}, {1,0}, {1,1}, {0,1});
    // top
    face(5,
         {-s, s, s}, { s, s, s}, { s, s,-s}, {-s, s,-s},
         {0,1,0}, {0,0}, {1,0}, {1,1}, {0,1});

    return v;
}

static std::vector<unsigned int> CubeIndices()
{
    std::vector<unsigned int> idx;
    for (unsigned int i = 0; i < 36; ++i) idx.push_back(i);
    return idx;
}

/* --------------------------------------------------------------------- */
/* 2. Game state (anonymous namespace)                                   */
/* --------------------------------------------------------------------- */
namespace {
    bool firstMouse = true;
    float lastX = 400.0f, lastY = 300.0f;
    float yaw = -90.0f, pitch = 0.0f;
    const float sensitivity = 0.1f;
    bool CursorEnabled = true;
    glm::vec3 forward;

    Mesh*   cube  = nullptr;
    Mesh*   cube2 = nullptr;
    Camera* userCamera = nullptr;

    const std::string wallTexture  = "resources/Textures/container.jpg";
    const std::string brickTexture = "resources/Textures/wall.jpg";
}

/* --------------------------------------------------------------------- */
/* 3. Engine callbacks                                                   */
/* --------------------------------------------------------------------- */
namespace PointEngine {
void Start()
{
    std::cout << "game started :>\n";

    userCamera = new Camera;
    userCamera->transform.position = glm::vec3(0, 0, 3);

    // Lights
    auto* pointLight  = new PointLight(glm::vec3(1.0f), 1.0f);
    auto* pointLight2 = new PointLight(glm::vec3(1.0f), 1.0f);
    pointLight->transform.position  = glm::vec3(0.0f, 1.0f, 0.0f);
    pointLight2->transform.position = glm::vec3(0.0f, 1.0f, 2.0f);
    AddPointLight(pointLight);
    AddPointLight(pointLight2);

    auto* dirLight = new DirectionalLight(
        glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.01f), glm::vec3(0.1f), glm::vec3(0.5f));
    AddDirectionalLight(dirLight);

    // TEXTURES
    Texture texWall  = LoadGLTexture(wallTexture,  "texture_diffuse");
    Texture texBrick = LoadGLTexture(brickTexture, "texture_diffuse");

    // CUBE 1
    static const auto verts = CubeVertices();
    static const auto inds  = CubeIndices();


    forward = glm::vec3(0, 0, -1);
    for (auto* obj : GetSceneObjects()) obj->Start();
}

void Update()
{
    forward = userCamera->GetForwardVector();

    for (auto* obj : GetSceneObjects()) obj->Update();

    if (cube)
        cube->transform.rotation.y += 20.0f * GetDeltaTime();
}

void End()
{
    RemoveObjs();
    std::cout << "game ended :<\n";
}

/* ---------- INPUT ---------- */
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    float x = static_cast<float>(xpos);
    float y = static_cast<float>(ypos);

    if (firstMouse) { lastX = x; lastY = y; firstMouse = false; }

    float xoffset = (x - lastX) * sensitivity;
    float yoffset = (lastY - y) * sensitivity;
    lastX = x; lastY = y;

    yaw   += xoffset;
    pitch += yoffset;
    if (pitch > 89.0f)  pitch = 89.0f;
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

void ProcessInput(GLFWwindow* window)
{
    if (!userCamera) return;

    float speed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                  ? 10.0f * GetDeltaTime()
                  :  5.0f * GetDeltaTime();

    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0,1,0)));
    glm::vec3 up    = glm::normalize(glm::cross(right, forward));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) userCamera->transform.position += forward * speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) userCamera->transform.position -= forward * speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) userCamera->transform.position -= right  * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) userCamera->transform.position += right  * speed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) userCamera->transform.position.y -= speed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) userCamera->transform.position.y += speed;

    static bool escLast = false;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (!escLast) {
            CursorEnabled = !CursorEnabled;
            glfwSetInputMode(window, GLFW_CURSOR,
                CursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
            firstMouse = true;
        }
        escLast = true;
    } else escLast = false;
}

Camera* GetUserCamera() { return userCamera; }

} // namespace PointEngine