#version 400 core

in vec3 ourColor; // set this variable in the OpenGL code

out vec4 color;

void main()
{
	color = vec4(ourColor, 1.0f);
}