#include <cstddef>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "engine.h"
#include "core/shader.h"
#include "core/stb_image.h"
#include "components/GameObject.h"
#include "components/Camera.h"
#include "components/Meshes/Cube.h"
#include "game/game.h"
#include "components/LightManager.h"

using namespace PointEngine;

// Internal engine state
namespace {
    double g_deltaTime = 0.0;
    const unsigned int WIDTH = 800;
    const unsigned int HEIGHT = 600;
    bool VsyncEnabled = false;
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
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "PointEngine", NULL, NULL);
    GLFWimage PointIcon[1];
    PointIcon[0].pixels = stbi_load("resources/Icon.png", &PointIcon[0].width, &PointIcon[0].height, 0, 4);
    glfwSetWindowIcon(window, 1, PointIcon);
    stbi_image_free(PointIcon[0].pixels);

    
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(VsyncEnabled ? 1 : 0); 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD :<" << std::endl;
        return -1;
    }

    Shader shader("./resources/shaders/shader.vs", "./resources/shaders/shader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, CubeVerticesSize * sizeof(float), CubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, CubeIndicesSize * sizeof(unsigned int), CubeIndices, GL_STATIC_DRAW);

    // 0: position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 1: normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 2: uv
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    glEnable(GL_DEPTH_TEST);
    double oldTimeSinceStart = 0.0;

    double fpsTimer = 0.0;
    int frames = 0;
    int fps = 0;
    
    unsigned int texture;
    double now;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    
    Start();

    // --- внутри main(), после Start() ---
while (!glfwWindowShouldClose(window))
{
    now = glfwGetTime();
    SetDeltaTime(now - oldTimeSinceStart);
    oldTimeSinceStart = now;

    fpsTimer += GetDeltaTime();
    frames++;

    if (fpsTimer >= 1.0) {
        fps = frames;
        frames = 0;
        fpsTimer = 0.0;
        std::cout << "FPS: " << fps << std::endl;
    }

    // Ввод и мышь
    glfwSetCursorPosCallback(window, mouse_callback);
    ProcessInput(window);
    Update();

    // Очистка экрана
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // Матрицы камеры
    Camera* cam = GetUserCamera();
    glm::mat4 view = cam->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

    // Активируем шейдер
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setVec3("viewPos", cam->transform.position);

    // Загружаем все источники света
    UploadLightsToShader(shader);

    // Рендерим все объекты
    for (auto obj : GetSceneObjects()) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, obj->transform.position);
        model = glm::rotate(model, glm::radians(obj->transform.rotation.x), glm::vec3(1,0,0));
        model = glm::rotate(model, glm::radians(obj->transform.rotation.y), glm::vec3(0,1,0));
        model = glm::rotate(model, glm::radians(obj->transform.rotation.z), glm::vec3(0,0,1));
        model = glm::scale(model, obj->transform.scale);
        shader.setMat4("model", model);

        // Если объект имеет материал с текстурой
        if (auto mesh = dynamic_cast<Mesh*>(obj)) {
            // Bind textures manually based on type
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr = 1;
            unsigned int heightNr = 1;

            for (unsigned int i = 0; i < mesh->textures.size(); i++) {
                glActiveTexture(GL_TEXTURE0 + i);

                std::string name = mesh->textures[i].type;
                std::string number;
                int textureUnit = i;

                if (name == "texture_diffuse") {
                    number = std::to_string(diffuseNr++);
                    shader.setInt(("material.diffuse" + number).c_str(), i);
                }
                else if (name == "texture_specular") {
                    number = std::to_string(specularNr++);
                    shader.setInt(("material.specular" + number).c_str(), i);
                }
                else if (name == "texture_normal") {
                    number = std::to_string(normalNr++);
                    shader.setInt(("material.normal" + number).c_str(), i);
                }
                else if (name == "texture_height") {
                    number = std::to_string(heightNr++);
                    shader.setInt(("material.height" + number).c_str(), i);
                }

                glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
            }

            // Optional: Set default texture units if you want fixed slots
            // shader.setInt("material.diffuse", 0);
            // shader.setInt("material.specular", 1);

            // Draw the mesh
            glBindVertexArray(mesh->VAO);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // Reset active texture
            glActiveTexture(GL_TEXTURE0);
        }

        PointEngine::DrawAll(shader.ID, VAO);

        // Рисуем объект
        obj->Draw(shader.ID, 0); // VAO у Mesh должен быть внутри Draw()
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}


    End();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader.ID);

    
    
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
