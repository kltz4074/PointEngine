#ifndef ENGINE_H
#define ENGINE_H

#include "glm/fwd.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
namespace PointEngine {

/**
 * @brief utility function to read a file into a string
 * @param filePath path to the file to read
 * @return contents of the file as a string, or empty string on error
 */
inline std::string ReadFileToString(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::in | std::ios::binary);
    if (!file) {
        std::cerr << "Could not open the file - '" << filePath << "'\n";
        return {};
    }

    std::ostringstream contents;
    contents << file.rdbuf();
    return contents.str();
}

inline glm::vec3 convertColor255To1(const glm::vec3 color) {
    return glm::vec3(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
};

inline glm::vec3 convertColor1To255(const glm::vec3 color) {
    return glm::vec3(color.r * 255.0f, color.g * 255.0f, color.b * 255.0f);
};

inline void calculateFPS();
/**
 * @brief get the current delta time (time since last frame)
 * @return delta time in seconds
 */
double GetDeltaTime();

/**
 * @brief set the delta time (internal use only (as i understand))
 * @param dt delta time in seconds
 */
void SetDeltaTime(double dt);

// Source - https://stackoverflow.com/q
// Posted by Swatcat, modified by community. See post 'Timeline' for change history
// Retrieved 2025-12-02, License - CC BY-SA 3.0
} // namespace PointEngine

#endif // ENGINE_H
