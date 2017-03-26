#version 400 core

layout (location = 0) in vec3 position; // the position variable has attribute position 0
layout (location = 1) in vec3 color;

out vec3 ourColor;

void main()
{
	gl_Position = vec4(position, 1.0f); // see how we directly give a vec3 to vec4's constructor (i.e. "Swizzling")
	ourColor = color;
}