#include "rplusplus.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <string>
#include <filesystem>
#include <thread>
#include <cstdio>
#include <memory>
#include <array>
#include <fstream>

void rpp::runSourceFile(std::string_view rFilePath, std::string_view outputDir) {
    rSrcFilePath = rFilePath;
    outputPath = outputDir;

    std::string command = "Rscript -e \"png(filename='" + std::string(outputDir) + "/graph.png')\"";

    for (const Parameter& parameter : parameters) {
        command.append(" -e \"");
        command.append(parameter.identifier + " <- " + std::to_string(parameter.value));
        command.append("\"");
    }

    std::ifstream rFileStream(rFilePath.data());

    std::string line;
    while (std::getline(rFileStream, line)) {
        if (line != "") {
            command.append(" -e \"");
            command.append(line);
            command.append("\"");
        }
    }

    command.append(" -e \"dev.off()\" ");

    system(command.c_str());
}

bool rpp::createWindow(const char* title, int width, int height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize glad\n";
        return false;
    }

    shader.load();
    shader.use();

    float vertices[] = {
        // positions  // texture coords
        1.0f,  1.0f,  1.0f, 1.0f, // top right
        1.0f, -1.0f,  1.0f, 0.0f, // bottom right
       -1.0f, -1.0f,  0.0f, 0.0f, // bottom left
       -1.0f,  1.0f,  0.0f, 1.0f  // top left
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int vertexBuffer, elementBuffer;
    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &elementBuffer);

    glBindVertexArray(vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    loadImage();

    glViewport(0, 0, width, height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    {
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.IniFilename = NULL;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    return true;
}

void rpp::loadImage() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load((outputPath + "/graph.png").c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else
        std::cout << "Failed to load texture" << std::endl;

    stbi_image_free(data);
}

void rpp::addParameter(std::string_view displayName, std::string_view identifier, int value, int min, int max) {
    parameters.emplace_back(displayName.data(), identifier.data(), value, min, max, ImGuiDataType_S32);
}

void rpp::addParameter(std::string_view displayName, std::string_view identifier, float value, float min, float max) {
    parameters.emplace_back(displayName.data(), identifier.data(), *reinterpret_cast<int*>(&value), *reinterpret_cast<int*>(&min), *reinterpret_cast<int*>(&max), ImGuiDataType_Float);
}

void rpp::addParameter(std::string_view displayName, std::string_view identifier, unsigned int value, unsigned int min, unsigned int max) {
    parameters.emplace_back(displayName.data(), identifier.data(), *reinterpret_cast<int*>(&value), *reinterpret_cast<int*>(&min), *reinterpret_cast<int*>(&max), ImGuiDataType_U32);
}

bool rpp::shouldClose() {
    return glfwWindowShouldClose(window);
}

void rpp::drawFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowSize({0.0f, 0.0f});
    ImGui::Begin("Parameters", nullptr, ImGuiWindowFlags_NoResize);

    for (Parameter& parameter : parameters) {
        ImGui::DragScalar(parameter.displayName.c_str(), parameter.type, (void*)&parameter.value, 1.0f, (void*)&parameter.min, (void*)&parameter.max);
    }

    if (ImGui::Button("Redraw")) {
        runSourceFile(rSrcFilePath, outputPath);
        loadImage();
    }

    ImGui::End();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, texture);

    shader.use();
    glBindVertexArray(vertexArray);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
}