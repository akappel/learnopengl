#version 400 core

layout (location = 0) in vec3 position; // the position variable has attribute position 0
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 ourTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	ourColor = color;
	ourTexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
}
