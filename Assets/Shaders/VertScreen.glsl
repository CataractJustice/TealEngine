#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

out vec2 TexCoord;

void main()
{
	TexCoord = uv;
	gl_Position = vec4(pos.x, pos.y, 0.0f, 1.0f);
}