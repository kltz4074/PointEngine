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
    glfwSwapInterval(1); 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD :<" << std::endl;
        return -1;
    }

    
    Shader shader("./resources/shaders/shader.vs", "./resources/shaders/shader.fs");
    float CubeVertices[] = {
        // position of vertices            //colors                      // texture position
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

        // Back face
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,

        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.0f,  1.0f, 1.0f,

        // Right face
        0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.5f,  1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f, 1.0f,

        // Top face
        -0.5f,  0.5f, -0.5f,  1.0f, 0.5f, 1.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.5f, 1.0f, 1.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 1.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.5f, 1.0f,  0.0f, 1.0f,

        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.3f, 0.3f, 0.3f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.6f, 0.6f, 0.6f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.6f, 0.6f, 0.6f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.9f, 0.9f, 0.9f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f
    };

    unsigned int CubeIndices[] = {
         // Front face
        0, 1, 2,
        2, 4, 5,

        // Back face
        6, 7, 8,
        8, 10, 11,

        // Left face
        12, 13, 14,
        14, 16, 17,

        // Right face
        18, 19, 20,
        20, 22, 23,

        // Top face
        24, 25, 26,
        26, 28, 29,

        // Bottom face
        30, 31, 32,
        32, 34, 35
    };


    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CubeIndices), CubeIndices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    glEnable(GL_DEPTH_TEST);
    Start();
    double oldTimeSinceStart = 0.0; // <-- double, не int

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
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("resources/Textures/wall.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    while (!glfwWindowShouldClose(window))
    {

        now = glfwGetTime();
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

        processInput(window); // input
        Update(); 

        glClearColor(0.027f, 0.0f, 0.341f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glBindTexture(GL_TEXTURE_2D, texture);

        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

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