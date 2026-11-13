#version 330 core
#define PI 3.1415926538
#define DitherResolution (256.0f)
#define Bias (0.025f)
in vec2 TexCoord;

layout (location = 0) out vec4 LightColor;

uniform float strength;
uniform float radius;
uniform vec3 direction;
uniform vec4 ambient;
uniform sampler2D PositionMap;
uniform sampler2D NormalMap;
uniform sampler2D Dither;

layout (std140) uniform CameraBlock
{
	mat4 PV;
	vec3 camDir;
    vec3 camPos;
};

float calcObscurity(float angle, float distance, vec3 position, vec3 normal, vec3 tangent, vec3 bitangent) 
{
	vec3 p = position + (tangent * cos(angle) + bitangent * sin(angle)) * distance * radius;

	vec4 sp = PV * vec4(p, 1.0f);
	sp /= sp.w;
	if(abs(sp.x) > 1.0f) return 0.0f;
	if(abs(sp.y) > 1.0f) return 0.0f;
	sp = sp * 0.5f + 0.5f;
	float depth = dot(direction, p) - dot(direction, texture(PositionMap, sp.xy).xyz) - Bias;
	if(depth > radius) return 0.0f;
	
	return max(depth / radius, 0.0f);
}

void main()
{
	vec3 position = texture(PositionMap, TexCoord).xyz;
	vec4 dither = texture(Dither, gl_FragCoord.xy / DitherResolution);
	vec3 normal = normalize(texture(NormalMap, TexCoord).xyz);
	vec3 tangent = normalize(cross(normal, vec3(0.12456789f, 1.12f, 0.0000031f)));
	vec3 bitangent = normalize(cross(normal, tangent));
	

	float occulision = 0.0f;

	occulision += calcObscurity(PI * 2.0f * dither.x, 0.333f, position, normal, tangent, bitangent);
	occulision += calcObscurity(PI * 2.0f * dither.y, 0.333f, position, normal, tangent, bitangent);
	occulision += calcObscurity(PI * 2.0f * dither.z, 0.333f, position, normal, tangent, bitangent);
	occulision += calcObscurity(PI * 2.0f * dither.w, 0.666f, position, normal, tangent, bitangent);
	occulision += calcObscurity(PI * 2.0f * dither.x + PI, 0.666f, position, normal, tangent, bitangent);
	occulision += calcObscurity(PI * 2.0f * dither.y + PI, 0.666f, position, normal, tangent, bitangent);
	occulision += calcObscurity(PI * 2.0f * dither.z + PI, 0.999f, position, normal, tangent, bitangent);
	occulision += calcObscurity(PI * 2.0f * dither.w + PI, 0.999f, position, normal, tangent, bitangent);
	occulision += calcObscurity(PI * 2.0f + PI, 0.999f, position, normal, tangent, bitangent);
	
	LightColor = ambient + vec4(vec3(-occulision * strength * 0.0625f), 1.0f);
}