#version 330 core

layout (location = 0) in uint position;
layout (location = 1) in uint normal;
layout (location = 2) in uint material;

out vec3 TexOffset;
out vec3 TexWeight;
out vec3 FragPos;
out vec4 ClipPos;
out vec3 Normal;

layout (std140) uniform CameraBlock
{
	mat4 PV;
	vec3 camDir;
	vec3 camPos;
};

uniform vec3 offset;

void main()
{
	const float inv1023 = 1.0 / 1023.0;

	TexOffset = vec3(
		float(material & 1023u) * 12.0f + 0.5f,
		float((material >> 10u) & 1023u) * 12.0f + 0.5f,
		float((material >> 20u) & 1023u) * 12.0f + 0.5f
	);

	int m = gl_VertexID - (gl_VertexID / 3) * 3;
	TexWeight = vec3(0.0);
	TexWeight[m] = 1.0;

	const float inv8191 = 1.0 / 8191.0;

	float edgeX = float(position & 63u);
	float edgeY = float((position >> 6u) & 63u);
	uint axis  = (position >> 12u) & 3u;
	float foffset = float((position >> 14u) & 262143u) * inv8191;

	vec3 fpos = vec3(edgeX, edgeY, edgeY);
	fpos = mix(vec3(foffset, edgeX, edgeY), fpos, float(axis != 0u));
	fpos = mix(fpos, vec3(edgeX, foffset, edgeY), float(axis == 1u));
	fpos = mix(fpos, vec3(edgeX, edgeY, foffset), float(axis == 2u));

	vec4 worldPos = vec4(fpos + offset, 1.0);
	FragPos = worldPos.xyz;

	gl_Position = PV * worldPos;
	ClipPos = gl_Position;

	vec3 n = vec3(normal & 1023u, (normal >> 10u) & 1023u, (normal >> 20u) & 1023u) * inv1023;
	n = n * 2.0f - 1.0f;
	Normal = normalize(n);
}