#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <GLFW/glfw3.h>
namespace PointEngine {

class Camera : public GameObject {
public:
    glm::mat4 GetViewMatrix();
    glm::vec3 GetForwardVector();
    void FlyCameraMovement(GLFWwindow* window, double xpos, double ypos, float sensitivity, Camera* userCamera, glm::vec3 forward, float basicSpeed, float RunningSpeed, bool CursorEnabled);
};

} // namespace PointEngine

#endif // CAMERA_H

