#include <glad/glad.h>
#include <learnopengl/camera.h>
#include <GLFW/glfw3.h>

#define DEBUG_MODE

const float SCR_WIDTH = 800.0f;
const float SCR_HEIGHT = 600.0f;
//����ʼλ������Ļ����
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;
bool firstMouse = true;
extern Camera camera;

int deltaTime;
// ---------------------------------
// ���̼���
// ---------------------------------
void handleKeyInput(GLFWwindow* window)
{
	// esc�˳�
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// ��� WSAD ǰ������ Space�� ��Shift��
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		//camera.ProcessKeyboard(FORWARD, deltaTime);
#ifdef DEBUG_MODE
		std::cout << "Pressed W" << std::endl;
#endif
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		//camera.ProcessKeyboard(BACKWARD, deltaTime);
#ifdef DEBUG_MODE
		std::cout << "Pressed S" << std::endl;
#endif
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		//camera.ProcessKeyboard(LEFT, deltaTime);
#ifdef DEBUG_MODE
		std::cout << "Pressed A" << std::endl;
#endif
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		//camera.ProcessKeyboard(RIGHT, deltaTime);
#ifdef DEBUG_MODE
		std::cout << "Pressed D" << std::endl;
#endif
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		//camera.ProcessKeyboard(RUN, deltaTime);
#ifdef DEBUG_MODE
		std::cout << "Pressed SHIFT" << std::endl;
#endif
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		//camera.ProcessKeyboard(WALK, deltaTime);
#ifdef DEBUG_MODE
		std::cout << "RELEASED SHIFT" << std::endl;
#endif
	}

	/*  ԭΪ���ƶ�������Ϊ����ģ���ƶ�
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		car.ProcessKeyboard(CAR_FORWARD, deltaTime);

		// ֻ�г�����������ʱ��ſ���������ת
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			car.ProcessKeyboard(CAR_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			car.ProcessKeyboard(CAR_RIGHT, deltaTime);

		if (isCameraFixed)
			camera.ZoomOut();
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		car.ProcessKeyboard(CAR_BACKWARD, deltaTime);

		// ͬ��
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			car.ProcessKeyboard(CAR_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			car.ProcessKeyboard(CAR_RIGHT, deltaTime);

		if (isCameraFixed)
			camera.ZoomIn();
	}
	*/
	// �ص�����������һ������ֻ�ᴥ��һ���¼���
	glfwSetKeyCallback(window, key_callback);
}

// ���̻ص�������ʹ��һ�ΰ���ֻ����һ���¼�
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//��Ծ
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		/* code */
#ifdef DEBUG_MODE
		std::cout << "Pressed SPACE" << std::endl;
#endif
	}
	//����
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		/* code */
#ifdef DEBUG_MODE
		std::cout << "Pressed R" << std::endl;
#endif
	}

}

/*
*���ʹ����������������
glfwSetCursorPosCallback(window, mouse_callback);
glfwSetMouseButtonCallback(window, mouseButton_callback);
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	//������꣬ʹ�䲻����Ļ����ʾ�ҿ��Բ����Ӵ����������ƶ�
*/

// ��갴��
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
#ifdef DEBUG_MODE
			std::cout << "Pressed MOUSE LEFT BUTTON" << std::endl;
#endif
			break;
		}
	return;
}
// ����ƶ�
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // ���귭ת�Զ�Ӧ����ϵ

	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);

}