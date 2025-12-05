#include "LightManager.h"

namespace PointEngine {

// === Глобальные списки света ===
std::vector<PointLight*> pointLights;
std::vector<DirectionalLight*> directionalLights;

// === Добавление света ===
void AddPointLight(PointLight* light) {
    pointLights.push_back(light);
}

void AddDirectionalLight(DirectionalLight* light) {
    directionalLights.push_back(light);
}

// === Получение списков ===
std::vector<PointLight*>& GetPointLights() {
    return pointLights;
}

std::vector<DirectionalLight*>& GetDirectionalLights() {
    return directionalLights;
}

// === Загрузка всех источников света в шейдер ===
void UploadLightsToShader(const Shader& shader) {
    // ---------- Directional Lights ----------
    int dirIndex = 0;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_WEIGHT = 1024;
    unsigned int dephMap;
    for (auto light : directionalLights) {
        std::string base = "dirLights[" + std::to_string(dirIndex) + "]";
        shader.setVec3(base + ".direction", light->transform.rotation);
        shader.setVec3(base + ".ambient", light->ambient);
        shader.setVec3(base + ".diffuse", light->diffuse);
        shader.setVec3(base + ".specular", light->specular);
        dirIndex++;
    }
    shader.setInt("numDirLights", dirIndex);

    // ---------- Point Lights ----------
    int pointIndex = 0;
    for (auto light : pointLights) {
        std::string base = "pointLights[" + std::to_string(pointIndex) + "]";
        shader.setVec3(base + ".position", light->transform.position);
        shader.setVec3(base + ".ambient", light->ambient);
        shader.setVec3(base + ".diffuse", light->diffuse);
        shader.setVec3(base + ".specular", light->specular);
        shader.setFloat(base + ".constant", light->constant);
        shader.setFloat(base + ".linear", light->linear);
        shader.setFloat(base + ".quadratic", light->quadratic);
        pointIndex++;
    }
    shader.setInt("numPointLights", pointIndex);
}

} // namespace PointEngine
