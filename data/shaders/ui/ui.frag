#version 130

uniform sampler2D objectTexture;
uniform bool textureGiven;
uniform vec4 secondaryColor;
uniform vec4 globalColor;
uniform bool vertexColorGiven;

in vec2 uv2;
in vec4 vertexColor2;

out vec4 outColor;

vec3 YCbCrToRGB(vec3 YCbCr)
{
	float Y = YCbCr.r;
	float Cb = YCbCr.g;
	float Cr = YCbCr.b;
	return vec3(Y + 1.402 * (Cr - 0.5),
	            Y - 0.34414 * (Cb - 0.5) - 0.71414 * (Cr - 0.5),
	            Y + 1.772 * (Cb - 0.5));
}

vec3 RGBToYCbCr(vec3 RGB)
{
	float R = RGB.r;
	float G = RGB.g;
	float B = RGB.b;
	return vec3(0.299 * R + 0.587 * G + 0.114 * B,
	            -0.1687 * R - 0.3313 * G + 0.5 * B + 0.5,
	            0.5 * R - 0.4187 * G - 0.0813 * B + 0.5);
}

vec3 lighten(vec3 color)
{
	vec3 YCbCr = RGBToYCbCr(color);
	YCbCr.r = clamp(YCbCr.r * 1.4, 0.0, 1.0);
	vec3 RGB = YCbCrToRGB(YCbCr);
	return RGB;
}

void main()
{
	vec4 inputColor = globalColor;
	if (vertexColorGiven)
	{
		inputColor = vertexColor2;
	}
	if (textureGiven)
	{
		inputColor *= texture(objectTexture, uv2);
	}
	vec3 min = inputColor.rgb;
	vec3 max = lighten(inputColor.rgb);
	outColor.rgb = min * (1.0 - secondaryColor.rgb) + max * secondaryColor.rgb;
	outColor.a = inputColor.a * secondaryColor.a;
}



