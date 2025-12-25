#include <cstddef>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "engine.h"
#include "GLFW/glfw3.h"
#include "core/shader.h"
#include "core/stb_image.h"
#include "components/GameObject.h"
#include "components/Camera.h"
#include "components/Mesh.h"
#include "game/game.h"
#include "components/LightManager.h"
#include "components/Meshes/Cube.h"
#include "components/skybox.h"

using namespace PointEngine;

namespace {
    double g_deltaTime = 0.0;
    const unsigned int WIDTH = 800;
    const unsigned int HEIGHT = 600;
    bool VsyncEnabled = false;
    bool AntiAliasing = true;
    unsigned int DefaultVBO, DefaultVAO, DefaultEBO;
    int width, height;
    // Framebuffer objects for offscreen rendering (created in main)
    unsigned int framebuffer = 0;
    unsigned int textureColorbuffer = 0;
    unsigned int rbo = 0;
}
void framebuffer_size_callback(GLFWwindow* window, int w, int h);
inline void RenderScene(Shader shader);
inline void RenderObjects(Shader shader);

namespace PointEngine {
    double GetDeltaTime() {
        return g_deltaTime;
    }
    
    void SetDeltaTime(double dt) {
        g_deltaTime = dt;
    }

    const unsigned int WIDTH = 800;
    const unsigned int HEIGHT = 600;

        double oldTimeSinceStart = 0.0;

    double fpsTimer = 0.0;
    int frames = 0;
    int fps = 0;
    
    double now;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (AntiAliasing) {
        glfwWindowHint(GLFW_SAMPLES, 8);
    }

    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(PointEngine::WIDTH, PointEngine::HEIGHT, "PointEngine", 0, NULL);
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

    glGenVertexArrays(1, &DefaultVAO);
    glGenBuffers(1, &DefaultVBO);
    glGenBuffers(1, &DefaultEBO);

    glBindVertexArray(DefaultVAO);

    glBindBuffer(GL_ARRAY_BUFFER, DefaultVBO);
    glBufferData(GL_ARRAY_BUFFER, CubeVerticesSize * sizeof(float), CubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DefaultEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, CubeIndicesSize * sizeof(unsigned int), CubeIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_MULTISAMPLE);
    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT3);
    
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, CubeVerticesSize * sizeof(float), CubeVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    Shader shader("./resources/shaders/shader.vs", "./resources/shaders/shader.fs");
    Shader skyboxShader("./resources/shaders/skybox/shader.vs", "./resources/shaders/skybox/shader.fs");
    Shader screenShader("./resources/shaders/framebuffer/framebuffers_screen.vs", "./resources/shaders/framebuffer/framebuffers_screen.fs");
    
    Start();

    std::vector<std::string> faces
    {
    "resources/Textures/skybox/right.jpg",
    "resources/Textures/skybox/left.jpg",
    "resources/Textures/skybox/top.jpg",
    "resources/Textures/skybox/bottom.jpg",
    "resources/Textures/skybox/front.jpg",
    "resources/Textures/skybox/back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);


    screenShader.use();
    screenShader.setInt("screenTexture", 0);
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

    glfwGetFramebufferSize(window, &width, &height);
    if (width <= 0 || height <= 0) {
        width = PointEngine::WIDTH;
        height = PointEngine::HEIGHT;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Camera* cam = GetUserCamera();

    while (!glfwWindowShouldClose(window))
    {
        calculateFPS();
        
        glfwGetFramebufferSize(window, &width, &height);
        
        glm::mat4 view = cam->GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 1000.0f);
        glm::mat4 viewSky = glm::mat4(glm::mat3(view));

        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetWindowSizeCallback(window, framebuffer_size_callback);
        ProcessInput(window);
        
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        skyboxShader.use();
        skyboxShader.setMat4("view", viewSky);
        skyboxShader.setMat4("projection", projection);
        glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);
        
        Update();
        
        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("viewPos", cam->transform.position);

        UploadLightsToShader(shader);
        RenderObjects(shader);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        screenShader.use();  
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);  

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    End();
    
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Update OpenGL viewport
    glViewport(0, 0, width, height);
    // Update stored sizes
    ::width = width;
    ::height = height;

    // If our FBO is created, resize its attachments to match the new size
    if (framebuffer != 0) {
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ::width, ::height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ::width, ::height);

        // Validate framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete after resize!" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

inline void RenderScene(Shader shader) {
    Camera* cam = GetUserCamera();
    glm::mat4 view = cam->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 1000.0f);
    glm::mat4 viewSky = glm::mat4(glm::mat3(view));
    RenderObjects(shader);
}

inline void RenderObjects(Shader shader) {
    for (auto obj : GetSceneObjects()) {
        shader.setMat4("model", obj->transform.GetMatrix());
        if (auto mesh = dynamic_cast<Mesh*>(obj)) {
            glActiveTexture(GL_TEXTURE0);
            if (mesh->material.DiffuseTextureID){
                glBindTexture(GL_TEXTURE_2D, mesh->material.DiffuseTextureID);
                shader.setInt("material.diffuse", 0);                
            }
            if (mesh->model.VAO) {
                glBindVertexArray(mesh->model.VAO);
            } else {
                glBindVertexArray(mesh->model.DefaultVAO);
            }
            

            if (mesh->material.DiffuseTextureID) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, mesh->material.DiffuseTextureID);
            }

            if (mesh->model.indices.size()) {
                glDrawElements(GL_TRIANGLES, mesh->model.indices.size(), GL_UNSIGNED_INT, 0);
            } else {
                glDrawElements(GL_TRIANGLES, CubeIndicesSize, GL_UNSIGNED_INT, 0);
            }
            glBindVertexArray(0);
        }
    }
}

inline void PointEngine::calculateFPS() {
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
}