#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "game/game.cpp"
#include "components/GameObject.cpp"
#include "engine.h"
#include "core/shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

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
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD :<" << std::endl;
        return -1;
    }

    
    Shader shader("resources/shaders/shader.vs", "resources/shaders/shader.fs");

    float CubeVertices[] = {
        // Front face
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // bottom-left front, red
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, // bottom-right front, green
        0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // top-right front, blue
        0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // top-right front, blue
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, // top-left front, yellow
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // bottom-left front, red

        // Back face
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, // bottom-left back, purpure
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, // bottom-right back, циан
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // top-right back, white
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // top-right back, white
        -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f, // top-left back, brown
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, // bottom-left back, purpure

        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.0f, // top-left front
        -0.5f,  0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // top-left back
        -0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f, // bottom-left back
        -0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.5f, // bottom-left front
        -0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.0f,

        // Right face
        0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 1.0f, // top-right front
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.5f, // top-right back
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.5f, // bottom-right back
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.5f,
        0.5f, -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // bottom-right front
        0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

        // Top face
        -0.5f,  0.5f, -0.5f,  1.0f, 0.5f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,
        -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.5f, 1.0f,

        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.3f, 0.3f, 0.3f,
        0.5f, -0.5f,  0.5f,  0.6f, 0.6f, 0.6f,
        0.5f, -0.5f,  0.5f,  0.6f, 0.6f, 0.6f,
        -0.5f, -0.5f,  0.5f,  0.9f, 0.9f, 0.9f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    Start();
    double oldTimeSinceStart = 0.0; // <-- double, не int

    double fpsTimer = 0.0;
    int frames = 0;
    int fps = 0;

    while (!glfwWindowShouldClose(window))
    {
        glUseProgram(shader.ID);
        double now = glfwGetTime(); // <-- тоже double
        deltaTime = now - oldTimeSinceStart;
        oldTimeSinceStart = now;

        fpsTimer += deltaTime;
        frames++;

        if (fpsTimer >= 1.0) {
            fps = frames;
            frames = 0;
            fpsTimer = 0.0;
            std::cout << "FPS: " << fps << std::endl;
        }

        processInput(window);
        Update();

        glClearColor(0.027f, 0.0f, 0.341f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);


        shader.use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        
        DrawAll(shader.ID, VAO);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

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

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
