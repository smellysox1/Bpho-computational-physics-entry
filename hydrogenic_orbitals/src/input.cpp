#include "input.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;

    glm::mat4 perspective = glm::perspective(2.0f, (float)windowWidth / (float)windowHeight, 0.015625f, 10.0f);
}

void cursor_pos_callback(GLFWwindow* window, double x, double y) {
    if (dragging) {
        zenith += (x - previousX) * 0.015625;
        azimuth += (y - previousY) * 0.015625;
    }

    previousX = x;
    previousY = y;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (!ImGui::GetIO().WantCaptureMouse) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS)
                dragging = true;
            else if (action == GLFW_RELEASE)
                dragging = false;
        }
    }
}

void scroll_callback(GLFWwindow* window, [[maybe_unused]]double dx, double dy) {
    glfwGetCursorPos(window, &cursorX, &cursorY);

    float cursorXf = (float)cursorX;
    float cursorYf = (float)windowHeight - (float)cursorY;

    distance += 1.0f;
    distance *= std::pow(zoomFactor, -dy);
    distance -= 1.0f;
}