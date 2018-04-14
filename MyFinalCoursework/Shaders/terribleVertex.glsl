#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 position;

out vec4 clipSpace;

void main()
{
	clipSpace = (projMatrix *  viewMatrix* modelMatrix) *vec4(position, 1.0);
	gl_Position = clipSpace;
}

