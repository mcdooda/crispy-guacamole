#version 130

uniform mat4 normalMatrix;
uniform sampler2D objectTexture;
uniform sampler2D objectBumpMap;
uniform vec4 color;

in vec4 normal2;
in vec2 uv2;

out vec4 outColor;

float getHeightFromColor(vec4 c)
{
	return (c.r + c.g + c.b) / 3.0;
}

vec4 getNormalFromBumpMap(vec2 uv)
{
	vec2 bumpMapSize = textureSize(objectBumpMap, 1);
	vec2 px = vec2((1.0 / bumpMapSize.x) / 4.0, 0);
	vec2 py = vec2(0, (1.0 / bumpMapSize.y) / 4.0);
	vec3 dx = vec3(px.x * 2.0, 0, getHeightFromColor(texture(objectBumpMap, uv + px)) - getHeightFromColor(texture(objectBumpMap, uv - px)));
	vec3 dy = vec3(0, py.y * 2.0, getHeightFromColor(texture(objectBumpMap, uv + py)) - getHeightFromColor(texture(objectBumpMap, uv - py)));
	vec3 normal = cross(dx, dy);
	return vec4(normalize(normal), 1.0);
}

void main()
{
	vec4 textureColor = texture(objectTexture, vec2(uv2.x, 1 - uv2.y));
	vec4 normal3 = normalMatrix * normalize(normalize(normal2) + getNormalFromBumpMap(vec2(uv2.x, 1 - uv2.y)));
	vec3 light = vec3(1, 0, 0);
	outColor = textureColor * color + vec4(vec3(clamp(-1, 0, dot(normal3.xyz, light))), 1.0) * textureColor.a * color.a;
}


