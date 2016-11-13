#version 130

uniform sampler2D fontTexture;
uniform vec4 color;
uniform vec4 backgroundColor;

in vec2 uv2;

out vec4 outColor;

void main()
{
	vec4 fontTextureColor = texture(fontTexture, uv2);
	outColor = fontTextureColor * color + backgroundColor * 0;
	outColor = mix(backgroundColor, outColor, outColor.a);
}


