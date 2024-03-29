#version 130

uniform mat4 modelMatrix;
uniform mat4 vpMatrix;

in vec4 position;
in vec2 uv;
in vec4 vertexColor;

out vec2 uv2;
out vec4 vertexColor2;

void main()
{
	gl_Position = position * modelMatrix * vpMatrix;
	uv2 = uv;
	vertexColor2 = vertexColor;
}


