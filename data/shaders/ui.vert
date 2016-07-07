#version 130

uniform mat4 modelMatrix;
uniform mat4 vpMatrix;

in vec4 position;
in vec2 uv;

out vec2 uv2;

void main()
{
	gl_Position = vpMatrix * modelMatrix * position;
	uv2 = uv;
}


