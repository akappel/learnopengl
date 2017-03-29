#version 400 core

in vec3 ourColor; // set this variable in the OpenGL code
in vec2 ourTexCoord;

out vec4 color;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;

void main()
{
	color = mix(texture(ourTexture0, ourTexCoord), texture(ourTexture1, ourTexCoord), 0.2);
	// color = texture(ourTexture, ourTexCoord) * vec4(ourColor, 1.0f);
	// color = vec4(ourColor, 1.0f);
}
