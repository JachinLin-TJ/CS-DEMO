#include <glad/glad.h>
#include <learnopengl/camera.h>
#include <GLFW/glfw3.h>

#define DEBUG_MODE

const float SCR_WIDTH = 800.0f;
const float SCR_HEIGHT = 600.0f;
//鼠标初始位置在屏幕中心
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;
bool firstMouse = true;
extern Camera camera;

int deltaTime;
// ---------------------------------
// 键盘监听
// ---------------------------------
void handleKeyInput(GLFWwindow* window)
{
	// esc退出
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// 相机 WSAD 前后左右 Space跳 左Shift跑
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

	/*  原为车移动，可作为人物模型移动
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		car.ProcessKeyboard(CAR_FORWARD, deltaTime);

		// 只有车车动起来的时候才可以左右旋转
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			car.ProcessKeyboard(CAR_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			car.ProcessKeyboard(CAR_RIGHT, deltaTime);

		if (isCameraFixed)
			camera.ZoomOut();
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		car.ProcessKeyboard(CAR_BACKWARD, deltaTime);

		// 同上
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			car.ProcessKeyboard(CAR_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			car.ProcessKeyboard(CAR_RIGHT, deltaTime);

		if (isCameraFixed)
			camera.ZoomIn();
	}
	*/
	// 回调监听按键（一个按键只会触发一次事件）
	glfwSetKeyCallback(window, key_callback);
}

// 键盘回调函数，使得一次按键只触发一次事件
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//跳跃
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		/* code */
#ifdef DEBUG_MODE
		std::cout << "Pressed SPACE" << std::endl;
#endif
	}
	//换弹
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		/* code */
#ifdef DEBUG_MODE
		std::cout << "Pressed R" << std::endl;
#endif
	}

}

/*
*如何使用以下两个函数：
glfwSetCursorPosCallback(window, mouse_callback);
glfwSetMouseButtonCallback(window, mouseButton_callback);
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	//捕获鼠标，使其不在屏幕中显示且可以不受视窗限制无限移动
*/

// 鼠标按键
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
// 鼠标移动
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // 坐标翻转以对应坐标系

	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);

}