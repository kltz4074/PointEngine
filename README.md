# PointEngine

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C++](https://img.shields.io/badge/Language-C%2B%2B-blue.svg)]()
[![Graphics: OpenGL](https://img.shields.io/badge/Graphics-OpenGL-lightgrey.svg)]()
[![Platform: Windows](https://img.shields.io/badge/Platform-Windows-brightgreen.svg)]()
[![Platform: macOS](https://img.shields.io/badge/Platform-macOS-9cf.svg)]()
[![Platform: Linux](https://img.shields.io/badge/Platform-Linux-orange.svg)]()

PointEngine is a lightweight, open-source 3D engine written in C++ using OpenGL. PointEngine is in the very early stages of development and aims to be fast, extensible, and scriptable (Python support planned).

---

Contents
- About
- Features
- Benchmarks
- Releases / Demo
- Getting started
- Example usage
- Roadmap
- Contributing
- License
- Author & Contact

---

About
-----
PointEngine focuses on fast 3D rendering and general systems that are useful for simulations (chemical reactions, particle systems) and games. It is intended to be cross-platform and simple to build using CMake.

Features
--------
Right now PointEngine is in its early stages. Planned and core characteristics:

- Minimal, modern C++ core (early prototype)
- Rendering backend: OpenGL
- Cross-platform: tested on Windows, macOS, Arch Linux
- Designed with simulation-focused workloads in mind (chemical reactions, particles)
- Future: Python scripting for game/simulation logic, more physics & GPU compute support

Benchmarks
----------
Hardware and measured framerates from early tests (engine is a prototype — numbers vary heavily by scene and driver):

———————————

OS: Arch Linux  
CPU: Ryzen 5 5000  
GPU: integrated  
RAM: 16gb ddr4
FPS: 4k - 11k

———————————

OS: Windows  
CPU: i7-13620H  
GPU: RTX 4060 (laptop)  
RAM: 32 GB DDR5  
FPS: 6k - 10k

———————————

OS: macOS  
GPU: M4  
RAM: 16 GB  
FPS: 1k - 6k

———————————

Releases & Demo
---------------
Prebuilt engine releases and demo games are available in the repository Releases tab. Demo video:



https://github.com/user-attachments/assets/4fe129e2-63c0-4b59-863f-967722da47e7



Getting started
---------------
Requirements
- C++17 (or newer) compiler
- CMake 3.26+ (or newer)
- OpenGL development headers / drivers
- Platform-specific dev packages (GLFW/SDL2, GLM, etc. — see repository CMakeLists for exact deps)

Quick build (typical)
1. Clone the repo:
   git clone https://github.com/kltz4074/PointEngine.git
   cd PointEngine

2. Create build dir and run CMake:
   mkdir build
   cd build
   cmake ..
   cmake --build . --config Release

3. Run the built binary (path will depend on platform/build):
   ./PointEngine        # or PointEngine.exe on Windows

Example usage
-------------
At this stage, the engine exposes an internal API (C++) for scene creation, rendering, and simple systems. A minimal pseudocode flow (future Python binding planned):

C++ (illustrative)
```cpp
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

```

Roadmap
-------
(early — will be expanded)
- [x] Stabilize C++ core and public API
- [ ] Add Python scripting binding for game/simulation logic
- [ ] Add physics integration and basic particle/chemical reaction modules
- [ ] GPU-accelerated compute for large-scale simulations
- [ ] Editor (PointEngineEditor) for scene building and asset management
- [ ] Documentation site (GitHub Pages) and tutorials
If you want to help shape the roadmap, open an issue or PR.

License
-------
PointEngine is released under the MIT License. See the LICENSE file for details.

Author & Contact
----------------
Author: [kltzqu](https://kltz4074.github.io/)







