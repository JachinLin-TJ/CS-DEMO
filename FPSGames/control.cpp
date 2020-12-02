#include"control.h"
// ---------------------------------
// ����/������
// ---------------------------------

void handleKeyInput(GLFWwindow* window)
{
    // esc�˳�
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //�ƶ�
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

    // �ص�����������һ������ֻ�ᴥ��һ���¼���
    glfwSetKeyCallback(window, key_callback);
}

// �����ص�������ʹ��һ�ΰ���ֻ����һ���¼�
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //Ԥ��������
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
#ifdef DEBUG_MODE
        std::cout << "Pressed R" << std::endl;
#endif
        ;
    }

}

// ����ƶ�
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //if (!isCameraFixed) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // ���귭ת�Զ�Ӧ����ϵ

    lastX = xpos;
    lastY = ypos;

    gunCamera.ProcessMouseMovement(xoffset, yoffset);
    //}
}
//��갴��
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            //���
#ifdef DEBUG_MODE
            std::cout << "Pressed MOUSE LEFT BUTTON" << std::endl;
#endif
            break;
        }
    return;
}
