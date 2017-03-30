#include "shader.h"

Shader::Shader(const GLchar * vertexShaderPath, const GLchar * fragmentShaderPath)
{
	// I would say the constructor shouldn't have IO logic in it, it should just rely on chars

	// 1. retrieve the vertex/ fragment source code from file path
	std::string vertexShaderCode;
	std::string fragmentShaderCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	// ensures ifstream objects can throw exceptions
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		vertexShaderFile.open(vertexShaderPath);
		fragmentShaderFile.open(fragmentShaderPath);
		std::stringstream vertexShaderStream, fragmentShaderStream;

		// read file's buffer contents into streams
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();

		// convert stream into GLchar array
		vertexShaderCode = vertexShaderStream.str();
		fragmentShaderCode = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" << std::endl;
	}

	const GLchar* vertexShaderCodeString = vertexShaderCode.c_str();
	const GLchar* fragmentShaderCodeString = fragmentShaderCode.c_str();

	// 2. compile the shaders
	GLuint vertexShaderId, fragmentShaderId;

	CreateShader(&vertexShaderId, vertexShaderCodeString, GL_VERTEX_SHADER);
	CheckForShaderErrors(vertexShaderId);

	CreateShader(&fragmentShaderId, fragmentShaderCodeString, GL_FRAGMENT_SHADER);
	CheckForShaderErrors(fragmentShaderId);

	// 3. shader program
	CreateShaderProgram(&this->ProgramId, vertexShaderId, fragmentShaderId);
	CheckForProgramErrors(this->ProgramId);

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteProgram(vertexShaderId);
	glDeleteProgram(fragmentShaderId);
}

void Shader::Use()
{
	glUseProgram(this->ProgramId);
}

void Shader::CreateShader(GLuint* id, const char* shader, GLenum type)
{
	*id = glCreateShader(type);
	glShaderSource(*id, 1, &shader, NULL);
	glCompileShader(*id);
}

void Shader::CreateShaderProgram(GLuint * id, GLuint vertexShaderId, GLuint fragmentShaderId)
{
	*id = glCreateProgram();
	glAttachShader(*id, vertexShaderId);
	glAttachShader(*id, fragmentShaderId);
	glLinkProgram(*id);
}

void Shader::CheckForShaderErrors(GLuint shaderId)
{
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shaderId, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void Shader::CheckForProgramErrors(GLuint programId)
{
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
}

GLuint Shader::GetProgramId()
{
	return this->ProgramId;
}