#version 130

uniform sampler2D objectTexture;
uniform vec4 color;

in vec2 uv2;

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
	vec4 textureColor = texture(objectTexture, vec2(uv2.x, uv2.y));
	vec3 min = textureColor.rgb;
	vec3 max = lighten(textureColor.rgb);
	outColor.rgb = min * (1.0 - color.rgb) + max * color.rgb;
	outColor.a = textureColor.a * color.a;
}



