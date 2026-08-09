#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>

extern int windowWidth, windowHeight;
extern float distance;

extern float azimuth, zenith;

extern glm::mat4 perspective;

inline double cursorX, cursorY, previousX, previousY;
inline bool dragging;

const float zoomFactor = 1.0625f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_pos_callback(GLFWwindow* window, double x, double y);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, [[maybe_unused]]double dx, double dy);
