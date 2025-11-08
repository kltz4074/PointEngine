#ifndef POINTENGINE_CAMERA_H
#define POINTENGINE_CAMERA_H

#include "GameObject.h"
#include <glm/glm.hpp>

namespace PointEngine {

class Camera : public GameObject {
public:
    glm::mat4 GetViewMatrix();
};

} // namespace PointEngine

#endif // POINTENGINE_CAMERA_H

