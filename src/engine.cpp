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
#include "game/game.h"
#include "components/LightManager.h"
#include "components/Meshes/Cube.h"
#include "components/skybox.h"

using namespace PointEngine;

// Internal engine state
namespace {
    double g_deltaTime = 0.0;
    const unsigned int WIDTH = 800;
    const unsigned int HEIGHT = 600;
    bool VsyncEnabled = false;
    bool AntiAliasing = true;
    unsigned int DefaultVBO, DefaultVAO, DefaultEBO;
    int width, height;

   
}

// Forward declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
inline void RenderScene(Shader shader);
inline void RenderObjects(Shader shader);
// Public API implementation
namespace PointEngine {
    double GetDeltaTime() {
        return g_deltaTime;
    }
    
    void SetDeltaTime(double dt) {
        g_deltaTime = dt;
    }

    const unsigned int WIDTH = 800;
    const unsigned int HEIGHT = 600;
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

    Shader shader("./resources/shaders/shader.vs", "./resources/shaders/shader.fs");
    Shader skyboxShader("./resources/shaders/skybox/shader.vs", "./resources/shaders/skybox/shader.fs");
    
    /*
    Shader DepthShader("resources/shaders/light/shadow_mapping.vs", "resources/shaders/light/shadow_mapping.fs");
    Shader debugDepthQuad("resources/shaders/light/shadowMapping/debug_quad/debug_quad.vs", "resources/shaders/light/shadowMapping/debug_quad/debug_quad.fs");
    */

    /*
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // shaders configuration uwu
    shader.use();
    shader.setInt("diffuseTexture", 0);
    shader.setInt("shadowMap", 1);
    debugDepthQuad.use();
    debugDepthQuad.setInt("depthMap", 0);
 */
    double oldTimeSinceStart = 0.0;

    double fpsTimer = 0.0;
    int frames = 0;
    int fps = 0;
    
    unsigned int texture;
    double now;
    
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

        glfwGetFramebufferSize(window, &width, &height);
        
        Camera* cam = GetUserCamera();
        glm::mat4 view = cam->GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 1000.0f);
        glm::mat4 viewSky = glm::mat4(glm::mat3(view));

        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetKeyCallback(window, key_callback);
        
        ProcessInput(window);
        glClearColor(0.01, 0.01 ,0.01, 1.0f);
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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    End();
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteProgram(shader.ID);
    
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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
            glBindTexture(GL_TEXTURE_2D, mesh->material.DiffuseTextureID);
            shader.setInt("material.diffuse", 0);                
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mesh->material.DiffuseTextureID);
            shader.setInt("material.specular", 1);                
            mesh->Draw(shader);
        }
    }
}