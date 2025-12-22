cmake_minimum_required(VERSION 3.26.0)
project(PointEngine
    VERSION 0.0.5
    DESCRIPTION "PointEngine - A simple 3D engine on OpenGL"
)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# some libraries

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(rapidobj
    GIT_REPOSITORY  https://github.com/guybrush77/rapidobj.git
    GIT_TAG         origin/master)

FetchContent_MakeAvailable(rapidobj)

find_package(OpenGL REQUIRED)

add_executable(PointEngine
    src/engine.cpp
    src/glad.c
    src/core/stb_image.cpp
    src/components/GameObject.cpp
    src/components/Camera.cpp
    src/components/Mesh.cpp
    src/components/LightManager.cpp
    src/components/Light/PointLight.cpp
    src/components/Light/DirectionalLight.cpp
    src/components/Meshes/Cube.cpp
    src/components/Model.cpp
    src/game/game.cpp
)

target_include_directories(PointEngine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${glm_SOURCE_DIR}        
)

target_link_libraries(PointEngine
    PRIVATE
        glfw
        OpenGL::GL
        glm
        rapidobj::rapidobj
)

target_include_directories(PointEngine PRIVATE ${assimp_SOURCE_DIR}/include)
