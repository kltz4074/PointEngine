#include "Camera.h"
#include "engine.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <GLFW/glfw3.h>
namespace PointEngine {

glm::mat4 Camera::GetViewMatrix() {
    glm::vec3 forward;
    forward.x = cos(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
    forward.y = sin(glm::radians(transform.rotation.x));
    forward.z = sin(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
    forward = glm::normalize(forward);
    
    return glm::lookAt(transform.position, transform.position + forward, glm::vec3(0, 1, 0));
}

glm::vec3 Camera::GetForwardVector() {
    glm::vec3 forward;
    forward.x = cos(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
    forward.y = sin(glm::radians(transform.rotation.x));
    forward.z = sin(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
    return glm::normalize(forward);
}

void Camera::FlyCameraMovement(
        GLFWwindow* window, 
        double xpos, double ypos, float sensitivity, 
        Camera* userCamera, glm::vec3 forward, 
        float basicSpeed, float RunningSpeed, bool CursorEnabled) {

    bool firstMouse = true;
    float lastX = 400.0f;
    float lastY = 300.0f;
    float yaw = -90.0f; // чтобы смотреть вперёд
    float pitch = 0.0f;
    float x = static_cast<float>(xpos);
    float y = static_cast<float>(ypos);
    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }
    
    float xoffset = x - lastX;
    float yoffset = lastY - y; // переворот оси Y
    lastX = x;
    lastY = y;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;
    
    // ограничиваем pitch
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.y = sin(glm::radians(pitch));
    forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(forward);
    if (userCamera) {
        userCamera->transform.rotation.y = yaw;
        userCamera->transform.rotation.x = pitch;
    }

    if (!userCamera) return;

    basicSpeed = 5.0f * PointEngine::GetDeltaTime();
    RunningSpeed = 10.0f * PointEngine::GetDeltaTime();
    float speed = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? RunningSpeed : basicSpeed;
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0, 1.0, 0.0)));
    glm::vec3 up = glm::normalize(glm::cross(right, forward));
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        userCamera->transform.position += forward * speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        userCamera->transform.position -= forward * speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        userCamera->transform.position -= right * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        userCamera->transform.position += right * speed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        userCamera->transform.position.y -= speed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        userCamera->transform.position.y += speed;
    static bool escPressedLastFrame = false;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (!escPressedLastFrame) {
            CursorEnabled = !CursorEnabled;
            glfwSetInputMode(window, GLFW_CURSOR,
                CursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        }
        firstMouse = true;
        escPressedLastFrame = true;
    }
    else {
        escPressedLastFrame = false;
    }



}

} // namespace PointEngine
