#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
#include <iostream>

#include "shader.h"
#include "camera.h"

int InitGLFWwindow();
int InitGLEW();

// object creation
void CreateTriangle(GLuint* id, GLfloat* vertices, GLuint size);
void CreateCube(GLuint* id, GLfloat* vertices, GLuint size);
void CreateRect(GLuint* id, GLfloat* vertices, GLuint* indices, GLuint sizeVertices, GLuint sizeIndices);
void CreateTexture(GLuint *textureId, char* filename, GLenum wrapType, GLenum texFilterType);
void DrawTriangle(GLuint* id);
void DrawCube(GLuint* id);
void DrawRect(GLuint* id);

// function callbacks
void KeyPressCB(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseMovementCB(GLFWwindow* window, double xpos, double ypos);
void ScrollCB(GLFWwindow* window, double xoffset, double yoffset);
void PerformKeyActions();

GLFWwindow* window;
GLfloat mixValue = 0.0f;
GLfloat yValue = 0.0f;

// global cam values
glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
// setup our camera
Camera camera(camPos, camUp);
GLfloat lastX = 400;
GLfloat lastY = 300;

// keys with multiple key presses
bool keys[1024];

// deltaTime framerate constraining
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLboolean firstMouse = true;

int main()
{
	GLfloat trigAVertices[] = {
		 // positions		// colors
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};
	GLfloat rectAVertices[] = {
		 // positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left 
	};
	GLfloat cubeAVertices[] = {
		// positions          // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	GLuint trigAId;
	GLuint rectAId;
	GLuint cubeAId;
	GLuint containerTexId;
	GLuint awesomefaceTexId;

	// boilerplate setup for GLFW window context and GLEW extension seeking
	if (InitGLFWwindow() == -1 || InitGLEW() == -1) {
		return -1;
	}
	
	// setup our shader for use
	Shader shader("./shader.vert", "./shader.frag");

	// setup viewport width and height based on retrieved values from GLFW
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Give a callback for handling keypresses
	glfwSetKeyCallback(window, KeyPressCB);
	glfwSetCursorPosCallback(window, MouseMovementCB);
	glfwSetScrollCallback(window, ScrollCB);

	// generate triangle VAOs
	CreateTriangle(&trigAId, trigAVertices, sizeof(trigAVertices));

	// generate rect VAO
	CreateRect(&rectAId, rectAVertices, indices, sizeof(rectAVertices), sizeof(indices));

	// generate cube VAO
	CreateCube(&cubeAId, cubeAVertices, sizeof(cubeAVertices));

	// setup textures
	CreateTexture(&containerTexId, "./container.jpg", GL_REPEAT, GL_LINEAR);
	CreateTexture(&awesomefaceTexId, "./awesomeface.png", GL_REPEAT, GL_LINEAR);

	while (!glfwWindowShouldClose(window)) {
		// calc deltaTime
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// check for events, such as a keypress
		glfwPollEvents();
		PerformKeyActions();

		// Rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();

		// set uniform mix value for the textures
		glUniform1f(glGetUniformLocation(shader.GetProgramId(), "mixValue"), mixValue);

		// setup multiple textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, containerTexId);
		glUniform1i(glGetUniformLocation(shader.GetProgramId(), "ourTexture0"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, awesomefaceTexId);
		glUniform1i(glGetUniformLocation(shader.GetProgramId(), "ourTexture1"), 1);

		// setup cube positions
		glm::vec3 cubePositions[] = {
			glm::vec3( 0.0f,  0.0f,  0.0f),
			glm::vec3( 2.0f,  5.0f, -15.0f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(-1.5f, -2.2f, -2.5f),			
			glm::vec3( 2.4f, -0.4f, -3.5f),
			glm::vec3( 1.5f,  2.0f, -2.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3( 1.3f, -2.0f, -2.5f),
			glm::vec3( 1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		// setup projection transform
		glm::mat4 projectionTransform;
		projectionTransform = glm::perspective(glm::radians(camera.Zoom), (GLfloat)width/height, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(shader.GetProgramId(), "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shader.GetProgramId(), "projection"), 1, GL_FALSE, glm::value_ptr(projectionTransform));

		// custom draw iteration for each position
		glBindVertexArray(cubeAId);
		for (GLuint i = 0; i < 10; i++) {
			glm::mat4 modelTransform;
			modelTransform = glm::translate(modelTransform, cubePositions[i]);
			GLfloat angle = glm::radians(20.0f * (i + 1));
			if (i % 2 == 0) {
				modelTransform = glm::rotate(modelTransform, (GLfloat)glfwGetTime() * angle, glm::vec3(1.0f, 0.3f, 0.5f));
			}
			else {
				modelTransform = glm::rotate(modelTransform, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			}
			glUniformMatrix4fv(glGetUniformLocation(shader.GetProgramId(), "model"), 1, GL_FALSE, glm::value_ptr(modelTransform));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// deactivate shader program
		glUseProgram(0);

		// display results of rendering
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void CreateTexture(GLuint *textureId, char* filename, GLenum wrapType, GLenum texFilterType)
{
	glGenTextures(1, textureId);
	glBindTexture(GL_TEXTURE_2D, *textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texFilterType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texFilterType);

	int texWidth, texHeight;
	unsigned char* image = SOIL_load_image(filename, &texWidth, &texHeight, 0, SOIL_LOAD_RGB);

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
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void CreateCube(GLuint* id, GLfloat* vertices, GLuint size)
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// texture attribute; jump straight to 2 since we're omitting color but still targeting the same attribute location
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// cleanup
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

	GLuint eboId;
	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices, indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// cleanup
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void DrawTriangle(GLuint* id)
{
	glBindVertexArray(*id);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void DrawCube(GLuint* id)
{
	glBindVertexArray(*id);
	glDrawArrays(GL_TRIANGLES, 0, 36);
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

	glEnable(GL_DEPTH_TEST);
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

void ScrollCB(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

void MouseMovementCB(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) // this bool variable is initially set to true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void KeyPressCB(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS) {
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		keys[key] = false;
	}

	if (action == GLFW_PRESS && key == GLFW_KEY_Q) {
		camera.GetCameraMode() == CameraMode::FPSMODE ? camera.SetCameraMode(CameraMode::FLYMODE) : camera.SetCameraMode(CameraMode::FPSMODE);
	}
}

void PerformKeyActions() {

	// change textures
	if (keys[GLFW_KEY_UP]) {
		mixValue > 1.0f ? mixValue = 1.0f : mixValue += 0.01f;
	}

	if (keys[GLFW_KEY_DOWN]) {
		mixValue < 0.0f ? mixValue = 0.0f : mixValue -= 0.01f;
	}

	// exit
	if (keys[GLFW_KEY_ESCAPE]) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// movement keys
	if (keys[GLFW_KEY_W]) {
		camera.ProcessMovement(CameraMovement::FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S]) {
		camera.ProcessMovement(CameraMovement::BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A]) {
		camera.ProcessMovement(CameraMovement::LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D]) {
		camera.ProcessMovement(CameraMovement::RIGHT, deltaTime);
	}
}