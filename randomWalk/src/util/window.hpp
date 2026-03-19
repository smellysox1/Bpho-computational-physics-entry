#pragma once

#include <GLFW/glfw3.h>

namespace window {
    inline bool draggingWindow = false;
    inline double initialCursorX, initialCursorY;

    inline bool maximised = false;
    inline int previousSizeX, previousSizeY;
    inline int previousPosX, previousPosY;

    void drag(GLFWwindow* win, double mouseX, double mouseY);

    void maximise(GLFWwindow* win);
    void minimise(GLFWwindow* 🪟);
}