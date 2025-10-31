#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "GLFW/glfw3.h" 
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

    Shader ourShader("resources/shaders/shader.vs", "resources/shaders/shader.fs"); // you can name your shader files however you like
    float TrinagleVertices[] = {
        0.0f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.0f,  0.5f,  0.0f
    };

float CubeVertices[] = {
    // Front face
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // bottom-left front, красный
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, // bottom-right front, зелёный
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // top-right front, синий
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // top-right front, синий
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, // top-left front, жёлтый
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // bottom-left front, красный

    // Back face
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, // bottom-left back, пурпурный
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, // bottom-right back, циан
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // top-right back, белый
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, // top-right back, белый
    -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f, // top-left back, серый
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, // bottom-left back, пурпурный

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
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    Start();
    double oldTimeSinceStart = 0.0; // <-- double, не int

    double fpsTimer = 0.0;
    int frames = 0;
    int fps = 0;

    GameObject* cube = new GameObject("Cube");
    cube->VAO = VAO;
    cube->vertexCount = 36;
    AddGameObject(cube);
    while (!glfwWindowShouldClose(window))
    {
    double now = glfwGetTime();
    deltaTime = now - oldTimeSinceStart;
    oldTimeSinceStart = now;

    processInput(window);
    UpdateAll();

    glClearColor(0.027f, 0.0f, 0.341f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // после создания шейдера
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH/HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(1.5f,1.5f,3.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));

    // перед циклом рендеринга
    unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
    unsigned int projLoc = glGetUniformLocation(ourShader.ID, "projection");

    ourShader.use();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    DrawAll(ourShader.ID);

    

    glfwSwapBuffers(window);
    glfwPollEvents();
        
    }
    End();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(ourShader.ID);

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