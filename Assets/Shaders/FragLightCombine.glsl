#version 330 core
layout (location = 0) out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D Unlit;
uniform sampler2D AlbedoMap;
uniform sampler2D LightMap;

void main()
{
	vec4 light = texture(LightMap, TexCoord);
	FragColor = vec4(texture(Unlit, TexCoord).xyz + max(vec3(0.0f, 0.0f, 0.0f), texture(AlbedoMap, TexCoord).xyz * light.xyz * light.w), 1.0f);
}