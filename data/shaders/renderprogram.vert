#version 130

uniform vec2 screenSize;
uniform vec2 arenaSize;

in vec2 position;
in vec2 uv;

out vec2 uv2;

void main()
{
	float x = position.x * arenaSize.x / arenaSize.y * screenSize.y / screenSize.x;
	float y = position.y;
	gl_Position = vec4(x, y, 0, 1);
	uv2 = uv;
}


