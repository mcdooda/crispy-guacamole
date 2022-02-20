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
	//outColor = vec4(uv2.x, uv2.y, 0.0, outColor.a);
	//outColor = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, outColor.a);
	//outColor = vec4(depth2, depth2, depth2, outColor.a);
}


