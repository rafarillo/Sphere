#define _USE_MATH_DEFINES

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <vector>
#include <math.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"


Window mainWindow;
// IBO = iondex bufffer array
const float toRadians = M_PI / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
GLfloat dTime = 0.0f;
GLfloat lastTime = 0.0f;

int direction = 1;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

// Vertex Shader

static const char* vShader = "Shaders/shader1.vert";

static const char* fShader = "Shaders/shader1.frag";

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CreateShpere(int n) // n is the number of faces
{
	unsigned int* indices = new unsigned int[3 * n * n]; // 3 because is 3 coordinates x, y, z
	GLfloat* vertices = new GLfloat[3 * n * n];
	float raiuds = 1.0f;
	int counter = 0;
	for (int i = 0; i < n; i++)
	{
		float theta = M_PI / n * (float)i;
		for (int j = 0; j < n; j++)
		{
			float phi = 2.0 * M_PI / n * (float)(j);
			float x = 1.0 * sin(theta) * cos(phi);
			float y = 1.0 * sin(theta) * sin(phi);
			float z = 1.0 * cos(theta);

			vertices[counter] = x;
			vertices[counter + 1] = y;
			vertices[counter + 2] = z;			
			counter += 3;
		}

	}

	// for(int i = 0; i < n*n; i++)
	// {
	// 	indices[i] = 0;
	// }

	int shift = 0;
	for (int i = 0; i < n*n; i++, shift += 3)
	{
		indices[shift] = i;
		indices[shift + 1] = (i + 1)%(3*n*n);
		indices[shift + 2] = (i + 2)%(3*n*n);
		
	}

	// for(int i = 0; i < n; i++)
	// {
	// 	for(int j = 0; j < n; j++)
	// 	{
	// 		indices[n*i + j] = n*i + j;
	// 		indices[(n*i + j + 1)%(n*n)] = (n*i + j + 1)%(n*n);
	// 		indices[(n*i + j + 2)%(n*n)] = (n*i + j + 2)%(n*n);

	// 	}
	// }


	/*printf("indices = \n");
	printf("[ \n");
	for (int i = 0; i < counter; i+=3)
	{
		printf("%d %d %d\n", indices[i], indices[i + 1], indices[i + 2]);
	}
	printf("]\n");*/

	Mesh* obj = new Mesh();
	obj->CreateMesh(vertices, indices, 3 * n * n, 3 * n * n);
	meshList.push_back(obj);
}

void CreateObject()
{
	unsigned int indices[] = {
		0, 3, 1, // ordem dos pontos para desenhar o triangulo
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);

	CreateShpere(15);

	/*Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);*/

}

int main()
{
	
	mainWindow = Window(800, 600);
	mainWindow.Intialise();

	CreateObject();
	CreateShaders();

	Camera cam = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 1.0f, 0.7f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	while (!mainWindow.getShouldClose())
	{
		GLfloat currentTime = glfwGetTime();
		dTime = currentTime - lastTime;
		lastTime = currentTime;
		// Get + Handle user input events
		glfwPollEvents();

		cam.KeyControl(mainWindow.getKeys(), dTime);
		cam.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());


		// clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(cam.CalculateViewMatix()));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		glUseProgram(0);

		// Swap the scene that is being drawn with the scene that is being seen
		mainWindow.SwapBuffers();
	}


	return 0;
}