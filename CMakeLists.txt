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
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# --- Assimp ---
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v5.4.3
)

# Assimp Build Settings
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)

# Enable Importers
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(assimp)

# Main Executable
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
    src/game/game.cpp
)

# Include Directories
target_include_directories(PointEngine
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${glm_SOURCE_DIR}
        ${assimp_SOURCE_DIR}/include
)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Link Libraries
target_link_libraries(PointEngine
    PRIVATE
        glfw
        OpenGL::GL
        glm
        assimp
)

# Platform-specific settings
if (WIN32)
    target_link_libraries(PointEngine
        PRIVATE
            mingw32
            msvcrt  # Optional, add if necessary
    )
endif()

if (UNIX AND NOT APPLE)
    find_package(Threads REQUIRED)
    target_link_libraries(PointEngine
        PRIVATE
            pthread
    )
endif()

# Compiler flags for cross-platform compatibility
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options(-DDEBUG)
else()
    add_compile_options(-O2)
endif()
