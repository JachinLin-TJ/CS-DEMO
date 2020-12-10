#include"include/glad/glad.h"
#include "GLFW/glfw3.h"
#include"utils.h"
extern Camera camera;
extern float deltaTime;
extern float lastX, lastY;
extern bool firstMouse;

#define DEBUG_MODE
void handleKeyInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods);