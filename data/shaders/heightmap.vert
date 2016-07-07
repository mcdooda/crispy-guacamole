#version 130

uniform mat4 modelMatrix;
uniform mat4 vpMatrix;

in vec4 position;
in vec4 normal;
in vec2 uv;

out vec4 normal2;
out vec2 uv2;

void main()
{
	gl_Position = vpMatrix * modelMatrix * position;
	normal2 = normal;
	uv2 = uv;
}


