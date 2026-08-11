#pragma once

#include <imgui.h>

#include <string_view>
#include <string>
#include <vector>

#include "shader.hpp"

struct GLFWwindow;

namespace rpp {
    struct Parameter {
        std::string displayName;
        std::string identifier;
        int value;                // use reinterpret cast for other data types
        int min, max;             // use reinterpret cast for other data types
        ImGuiDataType type;
    };

    inline std::string rSrcFilePath;
    inline std::string outputPath;
    inline GLFWwindow* window;
    inline std::vector<Parameter> parameters;
    inline rpp::Shader shader;
    inline unsigned int vertexArray;
    inline unsigned int texture;

    void runSourceFile(std::string_view rFilePath, std::string_view outputDir);
    bool createWindow(const char* title, int width, int height);
    void loadImage();
    bool shouldClose();
    void drawFrame();

    void addParameter(std::string_view displayName, std::string_view identifier, int value, int min, int max);
    void addParameter(std::string_view displayName, std::string_view identifier, float value, float min, float max);
    void addParameter(std::string_view displayName, std::string_view identifier, unsigned int value, unsigned int min, unsigned int max);
}