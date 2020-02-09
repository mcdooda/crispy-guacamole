#version 130

uniform sampler2D objectTexture;

in vec2 uv2;
in vec4 color2;
in float depth2;

out vec4 outColor;

void main()
{
	vec4 pxColor = texture(objectTexture, uv2);
	if (pxColor.a > 0.2 || (color2.r == 1.0 && color2.g == 1.0 && color2.b == 1.0 && color2.a == 1.0))
	{
		outColor = pxColor;
		if (outColor.a == 0.0)
		{
			discard;
		}
		outColor.a = color2.a;
	}
	else
	{
		vec2 pxSize = 1.0 / textureSize(objectTexture, 0);
		if (texture(objectTexture, vec2(uv2.x - pxSize.x, uv2.y)).a > 0.2
		 || texture(objectTexture, vec2(uv2.x + pxSize.x, uv2.y)).a > 0.2
		 || texture(objectTexture, vec2(uv2.x, uv2.y - pxSize.y)).a > 0.2
		 || texture(objectTexture, vec2(uv2.x, uv2.y + pxSize.y)).a > 0.2)
		{
			outColor = color2;
		}
		else
		{
			outColor = pxColor;
			discard;
		}
	}
	gl_FragDepth = depth2;
}


