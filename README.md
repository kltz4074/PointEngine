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



https://github.com/user-attachments/assets/9423ed5d-fb88-42ef-ae1a-99ef87963a29




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

Notes
- all needed libraries is in "include" folder ;)

Example usage
-------------
At this stage, the engine exposes an internal API (C++) for scene creation, rendering, and simple systems. A minimal pseudocode flow (future Python binding planned):

C++ (illustrative)
```cpp
#include <iostream>
#include "../components/GameObject.cpp"
#include "../engine.h"

bool CanUp = false;

class RotatingTriangle : public GameObject {
public:
    void Start() override {
        transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        transform.rotation = glm::vec3(0.0f);
    }

    void Update() override {
        float speed = 1.0f; // скорость движения (единица в секунду)
        float rotSpeed = 1.0f; // градусов в секунду

        if (CanUp)
            transform.position.y += speed * deltaTime;
        else
            transform.position.y -= speed * deltaTime;

        if (transform.position.y >= 0.5f)
            CanUp = false;
        if (transform.position.y <= -0.5f)
            CanUp = true;

        transform.rotation.y += rotSpeed * deltaTime;
        transform.rotation.z += rotSpeed * deltaTime;
    }
};


void Start() {
    std::cout << "game started :>\n";
    for (auto obj : sceneObjects) obj->Start();
    AddGameObject(new RotatingTriangle());
    
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

```

Roadmap
-------
(early — will be expanded)
- [ ] Stabilize C++ core and public API
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





