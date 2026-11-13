#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

out vec2 TexCoord;

uniform vec2 scale;
uniform vec2 offset;

void main()
{
	TexCoord = vec2(uv.x / 256.0f, uv.y);
	vec2 screenspace = vec2(pos.x * scale.x + offset.x, pos.y * scale.y + offset.y);
	screenspace *= 2.0f;
	screenspace -= 1.0f;
	gl_Position = vec4(screenspace.x, screenspace.y, 0.0f, 1.0f);
}