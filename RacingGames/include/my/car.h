#ifndef CAR_H
#define CAR_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <queue>

// 定义汽车移动的几个可能的选项
enum Direction {
	CAR_FORWARD,
	CAR_BACKWARD,
	CAR_LEFT,
	CAR_RIGHT
};

class Car {
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Left;
	float Yaw;
	float Pitch;
	// 存储旧Yaw信息，实现漂移
	queue<float> HistoryYaw;
	queue<float> HistoryPitch;
	int DelayFrameNum = 1;
	float DelayYaw;
	float DelayPitch;
	// 实现汽车缓动与缓停
	// TODO 设置一个speed属性，接收按键后根据deltatime不断增加或减小speed至阈值以实现缓动更加合适（而不是使用现在的奇怪的方法）（相机的Yaw偏移也相同）
	queue<glm::vec3> HistoryPosition;
	glm::vec3 DelayPosition;

	float MovementSpeed;
	float TurningSpeed;

	Car(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f))
		: MovementSpeed(30.0f)
		, TurningSpeed(90.0f)
		, Yaw(0.0f)
		, Pitch(0.0f)
		, DelayYaw(0.0f)
	{
		Position = position;
		updateFront();
		updateLeft();
	}

	glm::vec3 getPosition()
	{
		return Position;
	}

	float getYaw()
	{
		return Yaw;
	}
	float getPitch()
	{
		return Pitch;
	}
	float getDelayYaw()
	{
		return DelayYaw;
	}
	float getDelayPitch()
	{
		return DelayPitch;
	}
	float getYawDif()
	{
		return Yaw - DelayYaw;
	}
	float getPitchDif()
	{
		return Pitch - DelayPitch;
	}
	float getMidValYaw()
	{
		return (DelayYaw + Yaw) / 2;
	}
	float getMidValPitch()
	{
		return (DelayPitch + Pitch) / 2;
	}
	glm::vec3 getMidValPosition()
	{
		return (DelayPosition + Position) / 2.0f;
	}

	// 计算视图矩阵
	glm::mat4 GetViewMatrix(glm::vec3 cameraPosition)
	{
		return glm::lookAt(cameraPosition, Position, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	// 接受键盘输入
	void ProcessKeyboard(Direction direction, float deltaTime)
	{
		if (direction == CAR_FORWARD)
			Position += Front * MovementSpeed * deltaTime;
		if (direction == CAR_BACKWARD)
			Position -= Front * MovementSpeed * deltaTime;
		if (direction == CAR_LEFT)
			Position += Left * MovementSpeed * deltaTime;
		if (direction == CAR_RIGHT)
			Position -= Left * MovementSpeed * deltaTime;
		updateFront();
		updateLeft();
	}

	// 更新DalayYaw
	void UpdateDelayYaw()
	{
		HistoryYaw.push(Yaw);
		while (HistoryYaw.size() > DelayFrameNum) {
			HistoryYaw.pop();
		}
		DelayYaw = HistoryYaw.front();
	}
	void UpdateDelayPitch()
	{
		HistoryPitch.push(Pitch);
		while (HistoryPitch.size() > DelayFrameNum) {
			HistoryPitch.pop();
		}
		DelayPitch = HistoryPitch.front();
	}
	// 更新DalayYaw
	void UpdateDelayPosition()
	{
		HistoryPosition.push(Position);
		while (HistoryPosition.size() > DelayFrameNum) {
			HistoryPosition.pop();
		}
		DelayPosition = HistoryPosition.front();
	}
	// 接受鼠标移动
	void ProcessMouseMovement(float xoffset, float yoffset)
	{
		float MouseSensitivity = 0.1f;
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw -= xoffset;
		Pitch += yoffset;

		// 限制角度以避免翻转问题
		const float max_Pitch = 60.0f;
		const float min_Pitch = -60.0f;
		if (Pitch > max_Pitch)
			Pitch = max_Pitch;
		if (Pitch < min_Pitch)
			Pitch = min_Pitch;

		updateFront();
		updateLeft();
	}
private:
	// 计算新的 Front 向量
	void updateFront()
	{
		glm::vec3 front;
		front.x = -sin(glm::radians(getMidValYaw()));
		front.y = 0.0f;
		front.z = -cos(glm::radians(getMidValYaw()));
		Front = glm::normalize(front);
	}
	void updateLeft()
	{
		glm::vec3 left;
		glm::vec3 up(0, 1, 0);
		left = glm::cross(up, Front);
		Left = glm::normalize(left);
	}
};

#endif