#version 330 core

in vec2 TexCoord;

layout (location = 5) out vec4 LightColor;

//uniform sampler2D RenderTexture;
uniform sampler2D AlbedoMap;
uniform sampler2D PositionMap;
uniform sampler2D NormalMap;
uniform sampler2D SpecularMap;

uniform vec3 position;
uniform float radius;
uniform vec3 viewPos;
uniform vec4 color;
uniform vec3 CLQRatios;

void main()
{
	vec3 normal = texture(NormalMap, TexCoord).xyz;
	vec3 fragPos = vec3(texture(PositionMap, TexCoord));
	vec3 viewReflection = normalize(-reflect(fragPos - viewPos, normal));
	vec4 spectex = texture(SpecularMap, TexCoord);
	vec3 direction = fragPos - position;
	float distance = length(direction);
	vec3 CLQNormalized = CLQRatios / (CLQRatios.x + CLQRatios.y + CLQRatios.z);
	//Mixing constant/linear/quadratic light attenuation
	float attenuation = CLQNormalized.x + (1.0f / distance) * CLQNormalized.y + (1.0f / distance / distance) * CLQNormalized.z;
	float spec = pow(max(0.0f,dot(viewReflection, direction / distance)) * spectex.z, spectex.y) * spectex.x;
	float diffuse = max(0.0f,dot(-normal, direction / distance));
	LightColor = vec4(color.xyz*color.w, 1.0) * (spec + diffuse) * attenuation;
}