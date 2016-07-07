#version 130

uniform sampler2D screen;

in vec2 uv2;

out vec4 outColor;

void main()
{
	outColor = texture(screen, uv2);
}


