#include "window.hpp"


void window::drag(GLFWwindow* win, double mouseX, double mouseY) {
    if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        draggingWindow = false;
        return;
    }

    if (mouseY <= 96) {
        if (!draggingWindow) {
            initialCursorX = mouseX;
            initialCursorY = mouseY;
            draggingWindow = true;
        } else {
            int windowX, windowY;
            glfwGetWindowPos(win, &windowX, &windowY);
            glfwSetWindowPos(win, windowX + mouseX - initialCursorX, windowY + mouseY - initialCursorY);
        }
    }
}

void window::maximise(GLFWwindow* win) {
    if (window::maximised) {
        glfwSetWindowPos(win, window::previousPosX, window::previousPosY);
        glfwSetWindowSize(win, window::previousSizeX, window::previousSizeY);
    } else {
        glfwGetWindowSize(win, &window::previousSizeX, &window::previousSizeY);
        glfwGetWindowPos(win, &window::previousPosX, &window::previousPosY);
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
        glfwSetWindowSize(win, videoMode->width, videoMode->height);
        glfwSetWindowPos(win, 0, 0);
    }
    window::maximised = !window::maximised;
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include <GLFW/glfw3native.h>
    #include <windows.h>

    void window::minimise(GLFWwindow* 🪟) {
        HWND wind = glfwGetWin32Window(🪟);
        ShowWindow(wind, SW_MINIMIZE);
    }
#else // windows
    #include "logger.hpp"
    void window::minimise(GLFWwindow* 🪟) {
        WC2_ERROR("Get Windows");
    }
#endif // windows