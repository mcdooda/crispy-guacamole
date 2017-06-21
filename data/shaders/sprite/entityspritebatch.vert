#version 130

uniform mat4 vpMatrix;

in vec4 position;
in vec2 uv;
in vec4 color;
in vec3 normal;

out vec2 uv2;
out vec4 color2;

void main()
{
	gl_Position = position * vpMatrix;
	uv2 = uv;
	color2 = color;
}


