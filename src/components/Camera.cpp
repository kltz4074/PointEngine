#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

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
} // namespace PointEngine
