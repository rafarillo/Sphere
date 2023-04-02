#include "Shader.h"

Shader::Shader()
{
	shaderId = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);

	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::fstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		printf("Failed to read %s! File doesn't exist", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();

	return content;

}

GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

GLuint Shader::GetViewLocation()
{
	return uniformView;
}

void Shader::UseShader()
{
	glUseProgram(shaderId);
}

void Shader::ClearShader()
{
	if (shaderId != 0)
	{
		glDeleteProgram(shaderId);
		shaderId = 0;
	}
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
}

Shader::~Shader()
{
	ClearShader();
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderId = glCreateProgram();
	if (!shaderId)
	{
		printf("Error creating shader program \n");
		return;
	}

	AddShader(shaderId, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;

	GLchar errorLog[1024] = { 0 };

	glLinkProgram(shaderId);
	glGetProgramiv(shaderId, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderId, sizeof(errorLog), NULL, errorLog);
		printf("Error linking program: %s \n", errorLog);
		return;
	}

	glValidateProgram(shaderId);

	glLinkProgram(shaderId);
	glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderId, sizeof(errorLog), NULL, errorLog);
		printf("Error validating program: %s \n", errorLog);
		return;
	}

	glValidateProgram(shaderId);
	uniformModel = glGetUniformLocation(shaderId, "model");
	uniformProjection = glGetUniformLocation(shaderId, "projection");
	uniformView = glGetUniformLocation(shaderId, "view");
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;

	GLchar errorLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(errorLog), NULL, errorLog);
		printf("Error compiling the %d shader: %s \n", shaderType, errorLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}
