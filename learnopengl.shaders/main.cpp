#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int InitGLFWwindow();
int InitGLEW();

// object creation
void CreateTriangle(GLuint* id, GLfloat* vertices, GLuint size);
void CreateShader(GLuint* id, char* shader, GLenum type);
void CreateShaderProgram(GLuint* id, GLuint vertexShaderId, GLuint fragmentShaderId);

// some error handling
void CheckForShaderErrors(GLuint shaderId);
void CheckForProgramErrors(GLuint programId);

void DrawTriangle(GLuint* id);

// function callbacks
void CloseWindowCB(GLFWwindow* window, int key, int scancode, int action, int mode);

GLFWwindow* window;

int main() {
	GLfloat trigAVertices[] = {
		// positions		// colors
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	GLuint trigAId;
	GLuint vertexShaderId;
	GLuint orangeFragmentShaderId;
	GLuint orangeShaderProgramId;

	int width, height;
	char* vertexShader = {
		"#version 400 core\n"\

		"layout (location = 0) in vec3 position; // the position variable has attribute position 0\n"\
		"layout (location = 1) in vec3 color;"\

		"out vec3 ourColor;\n"\

		"void main()\n"\
		"{\n"\
		"  gl_Position = vec4(position, 1.0); // see how we directly give a vec3 to vec4's constructor (i.e. \"Swizzling\")\n"\
		"  ourColor = color;\n"\
		"}\n"
	};
	char* orangeFragmentShader = {
		"#version 400 core\n"\

		"in vec3 ourColor; // set this variable in the OpenGL code\n"\

		"out vec4 color;\n"\

		"void main()\n"\
		"{\n"\
		"  color = vec4(ourColor, 1.0f);\n"\
		"}\n"
	};


	// boilerplate setup for GLFW window context and GLEW extension seeking
	if (InitGLFWwindow() == -1 || InitGLEW() == -1) {
		return -1;
	}

	// setup viewport width and height based on retrieved values from GLFW
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Give a callback to close the window on ESC key press
	glfwSetKeyCallback(window, CloseWindowCB);

	// generate triangle VAOs
	CreateTriangle(&trigAId, trigAVertices, sizeof(trigAVertices));

	// setup shaders
	CreateShader(&vertexShaderId, vertexShader, GL_VERTEX_SHADER);
	CheckForShaderErrors(vertexShaderId);

	CreateShader(&orangeFragmentShaderId, orangeFragmentShader, GL_FRAGMENT_SHADER);
	CheckForShaderErrors(orangeFragmentShaderId);

	// setup program object to tie shaders together
	CreateShaderProgram(&orangeShaderProgramId, vertexShaderId, orangeFragmentShaderId);
	CheckForProgramErrors(orangeShaderProgramId);

	// delete the shaders, as they have now been linked into a program
	glDeleteShader(vertexShaderId);
	glDeleteShader(orangeFragmentShaderId);

	// setup wireframe mode
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		// check for events, such as a keypress
		glfwPollEvents();

		// Rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(orangeShaderProgramId);
		DrawTriangle(&trigAId);
		glUseProgram(0);

		// display results of rendering
		glfwSwapBuffers(window);
	}

	// clean up shader programs
	glDeleteProgram(orangeShaderProgramId);

	glfwTerminate();
	return 0;
}

void CreateTriangle(GLuint* id, GLfloat* vertices, GLuint size) {
	glGenVertexArrays(1, id);

	// we first need to bind the VAO
	glBindVertexArray(*id);

	// now generate, bind, and attribute our VBO
	GLuint vboId;
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// cleanup
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void DrawTriangle(GLuint* id) {
	glBindVertexArray(*id);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void CreateShader(GLuint* id, char* shader, GLenum type) {
	*id = glCreateShader(type);
	glShaderSource(*id, 1, &shader, NULL);
	glCompileShader(*id);
}

void CreateShaderProgram(GLuint* id, GLuint vertexShaderId, GLuint fragmentShaderId) {
	*id = glCreateProgram();
	glAttachShader(*id, vertexShaderId);
	glAttachShader(*id, fragmentShaderId);
	glLinkProgram(*id);
}

void CheckForShaderErrors(GLuint shaderId) {
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shaderId, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void CheckForProgramErrors(GLuint programId) {
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
}

int InitGLEW() {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
}

int InitGLFWwindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}


	glfwMakeContextCurrent(window);
}

void CloseWindowCB(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}