#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <glad/glad.h>

namespace PointEngine {

struct Transform {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    Transform();
    glm::mat4 GetMatrix() const;
};

class GameObject {
public:
    std::string name;
    Transform transform;

    GameObject(const std::string& name = "GameObject");
    virtual ~GameObject();

    virtual void Start();
    virtual void Update();
};

// Scene management functions
void AddGameObject(GameObject* obj);
void RemoveObjs();

// Get all scene objects (for iteration)
std::vector<GameObject*>& GetSceneObjects();

} // namespace PointEngine

#endif // GAMEOBJECT_H

