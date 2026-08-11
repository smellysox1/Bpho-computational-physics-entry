#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <filesystem>
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>

#include "shader.hpp"
#include "constants.hpp"

const std::string BASE_PATH = std::filesystem::current_path().parent_path().string() + '/';

size_t windowWidth = 800, windowHeight = 800;

Shader electronShader;

float d = 0.123e-9f;
float r = 65e-3f;
float acceleratingVoltage = 1.0f;

void errorCallback(int code, const char* description) {
    std::cerr << "GLFW error code " << code << ": " << description << '\n';
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
}

void limitFPS();

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwSetErrorCallback(errorCallback);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Electron Diffraction", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return 1;
    }

    electronShader.load(BASE_PATH + "electron_diffraction/shader/electron_diffraction.vert.glsl", BASE_PATH + "electron_diffraction/shader/electron_diffraction.frag.glsl");
    electronShader.use();

    float vertices[] = {
        1.0f,  1.0f, 0.0f,  // top right
        1.0f, -1.0f, 0.0f,  // bottom right
       -1.0f, -1.0f, 0.0f,  // bottom left
       -1.0f,  1.0f, 0.0f   // top left
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int vertexBufferObject, vertexArrayObject, elementBufferObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(1, &vertexBufferObject);
    glGenBuffers(1, &elementBufferObject);

    glBindVertexArray(vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);

    glViewport(0, 0, windowWidth, windowHeight);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    {
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.IniFilename = NULL;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        electronShader.use();

        float wavelength = h / std::sqrt(2000.0f * m_e * e * acceleratingVoltage);
        float maxN = d * 0.7653668648f / wavelength;

        electronShader.set1f("max_N", maxN);
        electronShader.set1f("V", acceleratingVoltage);

        glBindVertexArray(vertexArrayObject);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize({0.0f, 0.0f});
        ImGui::Begin("Electron Diffraction");

        ImGui::DragFloat("Accelerating Voltage (kV)", &acceleratingVoltage, 0.0625f, 1.0f, 5.0f);

        ImGui::End();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        limitFPS();
    }

    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &elementBufferObject);

    glfwTerminate();

    return 0;
}


void limitFPS() {
    // 60 frames per second, in nanoseconds
    static const unsigned int targetFrameLength = 16666667;

    static auto frameStartTime = std::chrono::steady_clock::now();

    auto now = std::chrono::steady_clock::now();
    auto diff = now - frameStartTime;

    frameStartTime = now;

    // prevent integer overflow
    if (diff.count() >= targetFrameLength)
        return;

    std::this_thread::sleep_for(std::chrono::nanoseconds(targetFrameLength) - diff);
}