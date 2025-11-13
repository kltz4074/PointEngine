#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include <vector>
#include "Light/PointLight.h"
#include "Light/DirectionalLight.h"
#include "../core/shader.h"

namespace PointEngine {

// === Глобальные списки света ===
extern std::vector<PointLight*> pointLights;
extern std::vector<DirectionalLight*> directionalLights;

// === Добавление света ===
void AddPointLight(PointLight* light);
void AddDirectionalLight(DirectionalLight* light);

// === Получение списков ===
std::vector<PointLight*>& GetPointLights();
std::vector<DirectionalLight*>& GetDirectionalLights();

// === Загрузка всех источников света в шейдер ===
void UploadLightsToShader(const Shader& shader);

} // namespace PointEngine

#endif // LIGHT_MANAGER_H
