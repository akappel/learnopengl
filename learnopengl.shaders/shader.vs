#version 400 core

layout (location = 0) in vec3 position; // the position variable has attribute position 0
layout (location = 1) in vec3 color;

out vec3 ourPosition;
out vec3 ourColor;

uniform float horizontalOffset;

void main()
{
	ourPosition = vec3(position.x + horizontalOffset, position.yz); // see how we directly give a vec3 to vec4's constructor (i.e. "Swizzling")
	gl_Position = vec4(ourPosition, 1.0f);
	ourColor = color;
}