#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>


class Camera
{
public:
	Camera(glm::vec3 p0, glm::vec3 up0, GLfloat yaw0, GLfloat pitch0, GLfloat movimentSpeed0, GLfloat turnSpeed0);
	~Camera();
	void KeyControl(bool *keys, GLfloat dTIme);
	void MouseControl(GLfloat xChange, GLfloat yChange);
	glm::mat4 CalculateViewMatix();

private:
	glm::vec3 position, front, up, right, worldUp;

	GLfloat yaw, pitch; // for rotation
	// yaw - left right rotation
	// pitch - up and down rotation

	GLfloat movimentSpeed;
	GLfloat turnSpeed;

	void update();
};

