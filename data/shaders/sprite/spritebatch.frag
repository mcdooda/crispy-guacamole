#version 130

uniform sampler2D objectTexture;

in vec2 uv2;
in vec4 color2;

out vec4 outColor;

void main()
{
	outColor = texture(objectTexture, vec2(uv2.x, uv2.y)) * color2;
}


