#ifndef POINTENGINE_H
#define POINTENGINE_H

/**
 * @file PointEngine.h
 * @brief Public API header for PointEngine
 * 
 * This is literally the header of main pubic API
 * Include this header to use the engine in other apps
 */

namespace PointEngine {

/**
 * @brief Get the current delta time (time since last frame)
 * @return Delta time in seconds
 */
double GetDeltaTime();

/**
 * @brief Game lifecycle functions
 */
void Start();
void Update();
void End();

/**
 * @brief Функция обработки ввода
 * @param window GLFW window handle (каст в GLFWwindow*)
 */
void ProcessInput(void* window);

/**
 * @brief Mouse callback function
 * @param window GLFW window handle (каст в GLFWwindow*)
 * @param xpos Мышь по x
 * @param ypos Мышь по y
 */
void mouse_callback(void* window, double xpos, double ypos);

} // namespace PointEngine

#endif // POINTENGINE_H

