#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>

#include "shader.h"

int InitGLFWwindow();
int InitGLEW();

// object creation
void CreateTriangle(GLuint* id, GLfloat* vertices, GLuint size);
void CreateRect(GLuint* id, GLfloat* vertices, GLuint* indices, GLuint sizeVertices, GLuint sizeIndices);
void CreateTexture(GLuint *texture);
void DrawTriangle(GLuint* id);
void DrawRect(GLuint* id);

// function callbacks
void CloseWindowCB(GLFWwindow* window, int key, int scancode, int action, int mode);

GLFWwindow* window;

int main()
{
	// boilerplate setup for GLFW window context and GLEW extension seeking
	if (InitGLFWwindow() == -1 || InitGLEW() == -1) {
		return -1;
	}

	GLfloat trigAVertices[] = {
		// positions		// colors
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};
	GLuint trigAId;

	GLfloat rectAVertices[] = {
		 // Positions          // Colors           // Texture Coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left 
	};
	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	GLuint rectAId;

	GLuint textureId;
	Shader shader("./shader.vs", "./shader.frag");

	// setup texture
	CreateTexture(&textureId);

	// setup viewport width and height based on retrieved values from GLFW
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Give a callback to close the window on ESC key press
	glfwSetKeyCallback(window, CloseWindowCB);

	// generate triangle VAOs
	CreateTriangle(&trigAId, trigAVertices, sizeof(trigAVertices));

	// generate rect VAO
	CreateRect(&rectAId, rectAVertices, indices, sizeof(rectAVertices), sizeof(indices));

	while (!glfwWindowShouldClose(window)) {
		// check for events, such as a keypress
		glfwPollEvents();

		// Rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();
		glUniform1f(glGetUniformLocation(shader.GetProgramId(), "horizontalOffset"), 0.0f);
		DrawTriangle(&trigAId);
		glUseProgram(0);

		// display results of rendering
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void CreateTexture(GLuint *texture)
{
	glGenTextures(1, texture);

	int texWidth, texHeight;
	unsigned char* image = SOIL_load_image("./container.jpg", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CreateTriangle(GLuint* id, GLfloat* vertices, GLuint size)
{
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

void CreateRect(GLuint* id, GLfloat* vertices, GLuint* indices, GLuint sizeVertices, GLuint sizeIndices)
{
	glGenVertexArrays(1, id);

	// we first need to bind the VAO
	glBindVertexArray(*id);

	// now generate, bind, and attribute our VBO
	GLuint vboId;
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeVertices, vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// finally, setup our indice EBO
	GLuint eboId;
	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices, indices, GL_STATIC_DRAW);

	// cleanup
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void DrawTriangle(GLuint* id)
{
	glBindVertexArray(*id);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void DrawRect(GLuint* id)
{
	glBindVertexArray(*id);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

int InitGLEW()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
}

int InitGLFWwindow()
{
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

void CloseWindowCB(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}