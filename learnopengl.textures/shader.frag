#version 400 core

in vec3 ourColor; // set this variable in the OpenGL code
in vec2 ourTexCoord;

out vec4 color;

uniform sampler2D ourTexture;

void main()
{
	color = texture(ourTexture, ourTexCoord);
	// color = vec4(ourColor, 1.0f);
}
