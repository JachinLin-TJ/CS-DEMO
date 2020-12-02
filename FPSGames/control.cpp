#include"control.h"
// ---------------------------------
// 键盘/鼠标监听
// ---------------------------------

void handleKeyInput(GLFWwindow* window)
{
    // esc退出
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //移动
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            gunCamera.ProcessKeyboard(_HIGHSPEED, deltaTime);
        else
            gunCamera.ProcessKeyboard(_FORWARD, deltaTime);
    }
       
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        gunCamera.ProcessKeyboard(_LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        gunCamera.ProcessKeyboard(_RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        gunCamera.ProcessKeyboard(_BACKWARD, deltaTime);

    // 回调监听按键（一个按键只会触发一次事件）
    glfwSetKeyCallback(window, key_callback);
}

// 按键回调函数，使得一次按键只触发一次事件
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //预留给换弹
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
#ifdef DEBUG_MODE
        std::cout << "Pressed R" << std::endl;
#endif
        ;
    }

}

// 鼠标移动
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //if (!isCameraFixed) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // 坐标翻转以对应坐标系

    lastX = xpos;
    lastY = ypos;

    gunCamera.ProcessMouseMovement(xoffset, yoffset);
    //}
}
//鼠标按键
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            //射击
#ifdef DEBUG_MODE
            std::cout << "Pressed MOUSE LEFT BUTTON" << std::endl;
#endif
            break;
        }
    return;
}
