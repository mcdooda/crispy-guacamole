#version 130

uniform sampler2D objectTexture;

in vec2 uv2;
in vec4 color2;
in float depth2;

out vec4 outColor;

void main()
{
	vec4 pxColor = texture(objectTexture, uv2);
	outColor = pxColor * color2;
	if (outColor.a == 0.0)
	{
		discard;
	}
	gl_FragDepth = depth2;
}


