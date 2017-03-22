#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

void CloseWindowCB(GLFWwindow* window, int key, int scancode, int action, int mode);
int InitGLFWwindow();
int InitGLEW();

GLFWwindow* window;

int main() {
	int width, height;

	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
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


	while (!glfwWindowShouldClose(window)) {
		// check for events, such as a keypress
		glfwPollEvents();

		// Rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// display results of rendering
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
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