#version 330 core
layout (location = 0) out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D UnlitColorMap;
uniform sampler2D PositionMap;
uniform sampler2D textureImage;
uniform vec3 CamPos;
uniform float density;

#define PI (3.14159)

void main()
{
	//should do uv sphere unvrap here
	vec3 d = normalize(texture(PositionMap, TexCoord).xyz - CamPos);
	vec4 fogColor = texture(textureImage, vec2(atan(d.z, d.x) / PI / PI, (asin(d.y) / PI) / 2.0f + 0.5f));
	float depth = distance(CamPos, texture(PositionMap, TexCoord).xyz);
	FragColor = mix(fogColor, texture(UnlitColorMap, TexCoord), exp(-density * depth * depth));
}
