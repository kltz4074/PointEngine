#ifndef ENGINE_H
#define ENGINE_H

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

inline std::string ReadFileToString(const std::string& filePath) // function to read a file
{
    std::ifstream file(filePath, std::ios::in | std::ios::binary);
    if (!file) {
        std::cerr << "Could not open the file - '" << filePath << "'\n";
        return {};
    }

    std::ostringstream contents;
    contents << file.rdbuf();
    return contents.str();
}

#endif // ENGINE_H