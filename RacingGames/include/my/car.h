#ifndef CAR_H
#define CAR_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <queue>

// ���������ƶ��ļ������ܵ�ѡ��
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
	// �洢��Yaw��Ϣ��ʵ��Ư��
	queue<float> HistoryYaw;
	queue<float> HistoryPitch;
	int DelayFrameNum = 1;
	float DelayYaw;
	float DelayPitch;
	// ʵ�����������뻺ͣ
	// TODO ����һ��speed���ԣ����հ��������deltatime�������ӻ��Сspeed����ֵ��ʵ�ֻ������Ӻ��ʣ�������ʹ�����ڵ���ֵķ������������Yawƫ��Ҳ��ͬ��
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

	// ������ͼ����
	glm::mat4 GetViewMatrix(glm::vec3 cameraPosition)
	{
		return glm::lookAt(cameraPosition, Position, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	// ���ܼ�������
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

	// ����DalayYaw
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
	// ����DalayYaw
	void UpdateDelayPosition()
	{
		HistoryPosition.push(Position);
		while (HistoryPosition.size() > DelayFrameNum) {
			HistoryPosition.pop();
		}
		DelayPosition = HistoryPosition.front();
	}
	// ��������ƶ�
	void ProcessMouseMovement(float xoffset, float yoffset)
	{
		float MouseSensitivity = 0.1f;
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw -= xoffset;
		Pitch += yoffset;

		// ���ƽǶ��Ա��ⷭת����
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
	// �����µ� Front ����
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