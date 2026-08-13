#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>
#include <cmath>

#include "arrows.hpp"
#include "lines.hpp"

int windowWidth = 1600, windowHeight = 800;

void GLFWErrorCallback(int code, const char* description) {
    std::cerr << "GLFW error code " << code << ": " << description << '\n';
}

void GLFWFrameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    windowWidth = width;
    windowHeight = height;

    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();

    glfwSetErrorCallback(GLFWErrorCallback);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Quantum Cryptography", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return 1;
    }

    glfwSetFramebufferSizeCallback(window, GLFWFrameBufferSizeCallback);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialise glad\n";
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    {
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.IniFilename = NULL;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    arrows::initArrows();
    lines::initLines();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Quantum Cryptography");

        static float theta, phi;

        ImGui::Text("Theta");
        ImGui::DragFloat("##theta", &theta, 1.0f, -90.0f, 90.0f);

        ImGui::Text("Phi");
        ImGui::DragFloat("##phi", &phi, 1.0f, -90.0f, 90.0f);

        float thetaRad = 0.017453292f * theta;
        float phiRad = 0.017453292f * phi;

        float cosTheta = std::cos(thetaRad);
        float sinTheta = std::sin(thetaRad);
        float cosPhi = std::cos(phiRad);
        float sinPhi = std::sin(phiRad);

        float sinThetaMinusPhi = std::sin(thetaRad - phiRad);

        ImGui::Text("Classical P(mismatch): ");
        ImGui::Text(std::to_string(1.0f - (cosTheta * cosTheta * cosPhi * cosPhi + sinTheta * sinTheta * sinPhi * sinPhi)).c_str());

        ImGui::Text("Quantum P(mismatch): ");
        ImGui::Text(std::to_string(sinThetaMinusPhi * sinThetaMinusPhi).c_str());

        ImGui::End();

        ImGui::Render();

        lines::drawLines();
        arrows::drawArrows(thetaRad, phiRad, windowWidth, windowHeight);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}