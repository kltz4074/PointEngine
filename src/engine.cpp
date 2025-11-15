// engine.cpp
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "engine.h"
#include "core/shader.h"
#include "core/stb_image.h"
#include "components/GameObject.h"
#include "components/Camera.h"
#include "components/Meshes/Cube.h"
#include "game/game.h"
#include "components/LightManager.h"

using namespace PointEngine;

// Internal engine state (anonymous namespace)
namespace {
    double g_deltaTime = 0.0;
    const unsigned int WIDTH = 800;
    const unsigned int HEIGHT = 600;
    bool VsyncEnabled = false;
    GLFWwindow* window;
}

// Forward declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Public API implementation
namespace PointEngine {
    double GetDeltaTime() {
        return g_deltaTime;
    }
    
    void SetDeltaTime(double dt) {
        g_deltaTime = dt;
    }

    GLFWwindow* GetGlfwWindow() {
        return window;
    }
}

// Helper: try to load icon, return true on success
static bool TryLoadWindowIcon(GLFWwindow* window, const char* path) {
    if (!window) return false;
    GLFWimage icon;
    icon.pixels = stbi_load(path, &icon.width, &icon.height, nullptr, 4);
    if (!icon.pixels) {
        std::cerr << "Warning: failed to load window icon: " << path << std::endl;
        return false;
    }
    glfwSetWindowIcon(window, 1, &icon);
    stbi_image_free(icon.pixels);
    return true;
}

int main()
{
    // --- GLFW init ---
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Request core profile 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // If you want transparent framebuffer (some compositors / platforms support it)
    // This allows alpha in glClearColor to be meaningful for window transparency.
    // Note: behavior is platform-dependent — keep if you plan transparent windows.
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(WIDTH, HEIGHT, "PointEngine", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    // Try set window icon (non-fatal)
    TryLoadWindowIcon(window, "resources/Icon.png");

    // Make context current and load GL loader
    glfwMakeContextCurrent(window);
    glfwSwapInterval(VsyncEnabled ? 1 : 0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD :<\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Set callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // cursor callback will be set once before loop (game provides mouse_callback)
    glfwSetCursorPosCallback(window, mouse_callback);

    // OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND); // включаем blending — полезно, если у тебя есть прозрачные материалы
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Build shader
    Shader shader("./resources/shaders/shader.vs", "./resources/shaders/shader.fs");

    // Setup VAO/VBO/EBO for a cube (shared simple mesh)
    unsigned int VBO = 0, VAO = 0, EBO = 0;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, CubeVerticesSize * sizeof(float), CubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, CubeIndicesSize * sizeof(unsigned int), CubeIndices, GL_STATIC_DRAW);

    // attrib 0: position (vec3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // attrib 1: normal (vec3)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // attrib 2: uv (vec2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // отвязываем

    // Optional: create a texture placeholder (not strictly necessary here)
    unsigned int placeholderTexture = 0;
    glGenTextures(1, &placeholderTexture);
    glBindTexture(GL_TEXTURE_2D, placeholderTexture);
    // default filtering/wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Start game-specific initialization
    Start();

    // Main loop timing
    double oldTimeSinceStart = glfwGetTime();
    double fpsTimer = 0.0;
    int frames = 0;
    int fps = 0;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        double now = glfwGetTime();
        double dt = now - oldTimeSinceStart;
        SetDeltaTime(dt);
        oldTimeSinceStart = now;

        fpsTimer += GetDeltaTime();
        frames++;

        if (fpsTimer >= 1.0) {
            fps = frames;
            frames = 0;
            fpsTimer = 0.0;
            std::cout << "FPS: " << fps << std::endl;
        }

        // Input & updates
        Update();

        // Clear frame (alpha=0 allows transparent framebuffer behaviour on supported platforms)
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Get framebuffer size
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        if (height == 0) height = 1; // защита от деления на ноль

        // Camera matrices
        Camera* cam = GetUserCamera();
        if (!cam) {
            std::cerr << "Warning: no user camera set. Skipping draw.\n";
        } else {
            glm::mat4 view = cam->GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);

            // Use shader & set common uniforms
            shader.use();
            shader.setMat4("view", view);
            shader.setMat4("projection", projection);
            shader.setVec3("viewPos", cam->transform.position);

            // Upload lights for this frame
            UploadLightsToShader(shader);

            // Render all objects
            for (auto obj : GetSceneObjects()) {
                if (!obj) continue;

                glm::mat4 model(1.0f);
                model = glm::translate(model, obj->transform.position);
                model = glm::rotate(model, glm::radians(obj->transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(obj->transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::rotate(model, glm::radians(obj->transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::scale(model, obj->transform.scale);
                shader.setMat4("model", model);

                // If object is a Mesh, bind its textures
                if (auto mesh = dynamic_cast<Mesh*>(obj)) {
                    // diffuse
                    glActiveTexture(GL_TEXTURE0);
                    unsigned int diffID = mesh->material.textureID ? mesh->material.textureID : placeholderTexture;
                    glBindTexture(GL_TEXTURE_2D, diffID);
                    shader.setInt("material.diffuse", 0);

                    // specular (if you use same texture as placeholder)
                    glActiveTexture(GL_TEXTURE1);
                    unsigned int specID = mesh->material.textureID ? mesh->material.textureID : diffID;
                    glBindTexture(GL_TEXTURE_2D, specID);
                    shader.setInt("material.specular", 1);
                }

                // Draw call (shared VAO used in simple DrawAll)
                PointEngine::DrawAll(shader.ID, VAO);

                // Per-object custom draw (object may already manage its own VAO inside Draw)
                obj->Draw(shader.ID, 0);
            }
        }

        // Swap & poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // End lifecycle
    End();

    // Cleanup GL objects
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
    if (placeholderTexture) glDeleteTextures(1, &placeholderTexture);
    if (shader.ID) glDeleteProgram(shader.ID);

    // Terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Update viewport on resize
    glViewport(0, 0, width, height);
}