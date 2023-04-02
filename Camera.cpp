#include "Camera.h"

Camera::Camera(glm::vec3 p0, glm::vec3 up0, GLfloat yaw0, GLfloat pitch0, GLfloat movimentSpeed0, GLfloat turnSpeed0)
{
	position = p0;
	worldUp = up0;
	yaw = yaw0;
	pitch = pitch0;

	movimentSpeed = movimentSpeed0;
	turnSpeed = turnSpeed0;

	update();
}

Camera::~Camera()
{

}

void Camera::KeyControl(bool* keys, GLfloat dTime)
{

	if (keys[GLFW_KEY_W])
	{
		position += front * movimentSpeed * dTime;
	}

	else if (keys[GLFW_KEY_S])
	{
		position -= front * movimentSpeed * dTime;
	}

	else if (keys[GLFW_KEY_D])
	{
		position += right * movimentSpeed * dTime;
	}

	else if (keys[GLFW_KEY_A])
	{
		position -= right * movimentSpeed * dTime;
	}

	else if (keys[GLFW_KEY_Q])
	{
		position -= up * movimentSpeed * dTime;
	}

	else if (keys[GLFW_KEY_E])
	{
		position += up * movimentSpeed * dTime;
	}
}


void Camera::MouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	else if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::CalculateViewMatix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));



}
