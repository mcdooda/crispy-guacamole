#version 130

uniform sampler2D fontTexture;
uniform vec4 backgroundColor;

in vec2 uv2;
in vec4 color2;

out vec4 outColor;

void main()
{
	vec4 fontTextureColor = texture(fontTexture, uv2);
	outColor = fontTextureColor * color2 + backgroundColor * 0;
	outColor = mix(backgroundColor, outColor, outColor.a);
}


