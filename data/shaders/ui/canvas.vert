#version 130

uniform mat4 viewMatrix;

in vec4 position;

void main()
{
	gl_Position = position * viewMatrix;
}


