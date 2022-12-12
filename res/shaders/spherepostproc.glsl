#version 330 core
layout (location = 0) out vec4 UnlitColor;

in vec2 TexCoord;

uniform sampler2D PositionMap;
uniform sampler2D LitMap;
uniform vec3 position;
uniform float radius;
uniform float thickness;
uniform vec4 color = vec4(1.0f);
uniform vec4 innerStart = vec4(0.0f);
uniform vec4 innerEnd = vec4(0.1f);

void main()
{
	vec3 fragPos = texture(PositionMap, TexCoord).xyz;
	float r = length(position - fragPos);
	float s1 = step(abs(r - radius), thickness);
	UnlitColor = texture(LitMap, TexCoord) * (1.0f - s1) + color * s1 + step(r, radius) * mix(innerStart, innerEnd, r / radius);
}