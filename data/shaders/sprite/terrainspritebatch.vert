#version 130

#define DEBUG_NORMALS 0

uniform mat4 vpMatrix;

in vec4 position;
in vec2 uv;
in vec4 color;
in vec3 normal;

out vec2 uv2;
out vec4 color2;

void main()
{
	gl_Position = position * vpMatrix;
	uv2 = uv;

	vec3 cameraDirection = normalize(vec3(-1.0, -1.0, -1.0));
	vec4 black = vec4(0.5, 0.5, 1.0, 1.0);
	vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
	float v = dot(normal, cameraDirection) / 2.0 + 0.5;

	// 1.25 - v to only darken back faces
	// 1.0 - v to also lighten front faces
	vec4 normalColor = mix(black, white, clamp(1.25 - v, 0.0, 1.0));

	color2 = color * normalColor;
#if DEBUG_NORMALS
	color2 = vec4(normal / 2.0 + vec3(0.5, 0.5, 0.5), 1.0);
#endif
}


