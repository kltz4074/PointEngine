#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace PointEngine {

class Camera : public GameObject {
public:
    glm::mat4 GetViewMatrix();
    glm::vec3 GetForwardVector();
};

} // namespace PointEngine

#endif // CAMERA_H

