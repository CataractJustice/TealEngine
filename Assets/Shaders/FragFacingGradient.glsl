#version 330 core
layout (location = 0) out vec4 UnlitColor;
layout (location = 1) out vec4 FragColor;
layout (location = 2) out vec3 FragPosition;
layout (location = 3) out vec3 FragNormal;
layout (location = 4) out vec4 FragSpecular;
layout (location = 5) out vec4 FragLight;

in vec2 TexCoord;
in vec3 FragPos;
in mat3 TBN;
in vec4 ClipPos;

uniform vec4 frontColor;
uniform vec4 sideColor;
uniform sampler2D AlbedoMap;
uniform sampler2D UnlitMap;
uniform sampler2D LightMap;

layout (std140) uniform CameraBlock
{
	mat4 PV;
	vec3 camDir;
    vec3 camPos;
};

void main()
{
	vec2 clipPos = ClipPos.xy / ClipPos.w * 0.5f + 0.5f;
    FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	FragPosition = FragPos;
	vec3 ln = vec3(0.0f, 0.0f, 1.0f);
    vec3 normal = normalize(TBN * ln);
	FragNormal = normal;
	FragSpecular = vec4(1.0f, 0.1f, 0.1f, 1.0f);
	FragLight = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec3 fragDir = normalize(camPos - FragPosition);
	vec4 um = texture(UnlitMap, clipPos);
	vec4 bgColor = (vec4(1.0f) - um) * um + abs(texture(AlbedoMap, clipPos) - texture(AlbedoMap, clipPos + vec2(0.001f, 0.001f))) * 2.0f;//( + texture(AlbedoMap, clipPos) * texture(LightMap, clipPos));
    UnlitColor = mix(frontColor, bgColor, pow(dot(fragDir, normal), 0.25f));
}