cmake_minimum_required(VERSION 3.26.0)
project(PointEngine)

include(FetchContent)

# --- GLFW ---
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)

# --- GLM ---
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1 # свежая и стабильная версия не трожь пока не нужны будут фичи из >=1.0.2
)
FetchContent_MakeAvailable(glm)

find_package(OpenGL REQUIRED)

add_executable(PointEngine 
    src/engine.cpp
    src/glad.c
    src/core/stb_image.cpp
    src/components/GameObject.cpp
    src/components/Camera.cpp
    src/components/Mesh.cpp
    src/components/Light/PointLight.cpp
    src/components/Light/DirectionalLight.cpp
    src/components/Meshes/Cube.cpp
    src/game/game.cpp
)

target_include_directories(PointEngine 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(PointEngine 
    PRIVATE
        glfw 
        OpenGL::GL
        glm
)

target_include_directories(PointEngine PRIVATE ${glm_SOURCE_DIR})