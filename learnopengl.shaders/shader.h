#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL\glew.h>

class Shader
{
public:
	// constructor reads and builds the shader
	Shader(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath);

	// use the program
	void Use();

	GLuint GetProgramId();

private:
	GLuint ProgramId;
	void CreateShader(GLuint* id, const char* shader, GLenum type);
	void CreateShaderProgram(GLuint* id, GLuint vertexShaderId, GLuint fragmentShaderId);
	void CheckForShaderErrors(GLuint shaderId);
	void CheckForProgramErrors(GLuint programId);
};