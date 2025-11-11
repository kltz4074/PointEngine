#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../components/GameObject.h"
#include "../components/Camera.h"
#include "../components/Mesh.h"
#include "../components/Light/PointLight.h"
#include "../components/Light/DirectionalLight.h"
#include "../engine.h"

namespace PointEngine {

// Game lifecycle functions
void Start();
void Update();
void End();

// Input processing
void ProcessInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Accessors for game objects (internal use only (as i understand))
Camera* GetUserCamera();
PointLight* GetPointLight();
DirectionalLight* GetDirectionalLight();
} // namespace PointEngine

#endif // GAME_H

