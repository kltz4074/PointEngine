cmake_minimum_required(VERSION 3.26.0)
project(PointEngine VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

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
    GIT_TAG 1.0.1 # можно заменить на нужную версию
)
FetchContent_MakeAvailable(glm)

find_package(OpenGL REQUIRED)

add_executable(PointEngine 
    src/engine.cpp
    src/glad.c
)

target_include_directories(PointEngine 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(PointEngine 
    PRIVATE
        glfw 
        OpenGL::GL
        glm
)

# Для корректного include <glm/glm.hpp>
target_include_directories(PointEngine PRIVATE ${glm_SOURCE_DIR})
