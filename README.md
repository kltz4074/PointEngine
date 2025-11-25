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
- Getting started
- Roadmap
- License
- Author & Contact

---

About
-----
PointEngine is planned to have:

- Photorealistic light simulation (like Unreal Engine)
- Realistic physics
- Flexible component system (like Unity :> )
- Easy coding API

---
Getting started
---------------
Requirements
- C++17 (or newer) compiler
- CMake 3.26+ (or newer)
- OpenGL development headers / drivers

build variants:

Windows:
   - git clone https://github.com/kltz4074/PointEngine
   - cd PointEngine
   - mkdir build
   - cd build
   - cmake ..
   - cmake --build .. --config Release
   - cd ..
   - start build/PointEngine.exe

Linux / MacOS:
   - git clone https://github.com/kltz4074/PointEngine
   - cd PointEngine
   - cmake .
   - make
   - ./PointEngine

---

Roadmap
-------
(early — will be expanded)
- [x] Stabilize C++ core and public API
- [ ] Photorealistic light system
- [ ] Add Python scripting binding for game logic
- [ ] Add physics integration and basic particle/chemical reaction modules
- [ ] Editor (PointEngineEditor) for scene building and asset management
- [ ] Documentation site (GitHub Pages) and tutorials
If you want to help shape the roadmap, open an issue or PR.

License
-------
PointEngine is released under the MIT License. See the LICENSE file for details.

Author & Contact
----------------
Author: [kltzqu](https://kltz4074.github.io/)














