#version 330 core

layout (location = 0) out vec4 UnlitColor;
layout (location = 1) out vec4 LitColor;

in vec2 TexCoord;

uniform sampler2D Unlit;
uniform isampler2D IdMap;

uniform int id;
uniform vec4 color;

#define stepSize 0.004

void main()
{
	if(
		(texture(IdMap, TexCoord + vec2(stepSize, 0.0f)).r == id ||
		texture(IdMap, TexCoord + vec2(-stepSize, 0.0f)).r == id || 
		texture(IdMap, TexCoord + vec2(0.0f, stepSize)).r == id ||
		texture(IdMap, TexCoord + vec2(0.0f, -stepSize)).r == id) &&
		texture(IdMap, TexCoord).r != id
	) 
	{
		UnlitColor = color;
		LitColor = vec4(0.0, 0.0, 0.0, 0.0);
	}
	else 
	{
		discard;
	}
}