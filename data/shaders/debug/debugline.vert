#version 130

uniform mat4 vpMatrix;

in vec4 position;
in vec4 color;

out vec4 color2;

void main()
{
	gl_Position = position * vpMatrix;
	color2 = color;
}


