#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	bufferHeight = 0;
	bufferWidth = 0;
	mainWindow = nullptr;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}

}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	height = windowHeight;
	width = windowWidth;
	bufferHeight = 0;
	bufferWidth = 0;
	mainWindow = nullptr;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}

}

int Window::Intialise()
{
	// initialize GLFW

	if (!glfwInit())
	{
		printf("GLFW initialization failed");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OPENGL version

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // version >= 3.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // version <= 3.0
	// Core profile = no backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // version <= 3.0
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // version <= 3.0

	mainWindow = glfwCreateWindow(width, height, "MandelBrot", NULL, NULL);
	if (!mainWindow)
	{
		printf("Creation of mainWindow failed");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// handle key + mouse input
	CreateCallbacks();

	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // sumindo com o curso, como num fps

	// Set the context for GLEW to use
	// everything must be draw in this window
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Failed glew initialization");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);

	return 0;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

void Window::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*> (glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("Pressed %d", key);
		}

		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("Released %d", key);

		}
	}
}

void Window::CreateCallbacks()
{
	glfwSetKeyCallback(mainWindow, HandleKeys);
	glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

void Window::HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*> (glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

	// printf("x:%.6f , y:%.6f\n", theWindow->xChange, theWindow->yChange);
}

GLfloat Window::GetXChange()
{
	GLfloat theChange = xChange;
	xChange = 0;
	return theChange;
}
GLfloat Window::GetYChange()
{
	GLfloat theChange = yChange;
	yChange = 0;
	return theChange;
}