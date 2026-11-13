#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec3 normal;

out vec2 TexCoord;
out vec3 FragPos;
out vec4 ClipPos;
out mat3 TBN;

layout (std140) uniform CameraBlock
{
	mat4 PV;
	vec3 camDir;
    vec3 camPos;
};

uniform vec3 offset;

vec3 unpacknorm(uint packedNormal) 
{
	float x = float((packedNormal >> 8u) & 0xFFu) / 255.0 * 2.0 - 1.0;
	float y = float(packedNormal & 0xFFu) / 255.0 * 2.0 - 1.0;
	vec3 n = vec3(x, y, 1.0 - abs(x) - abs(y));
	if (n.z < 0) {
		n.xy = (1.0 - abs(n.yx)) * sign(n.xy);
	}
	return normalize(n);
}

vec2 unpackuv(uint packedUV) 
{
	return vec2(
		float(packedUV & 0xFFFFu) / 65535.0f,
		float((packedUV >> 16u) & 0xFFFFu) / 65535.0f
	);
}

void main()
{
	//TexCoord = unpackuv(UVTBN.x);
	TexCoord = uv;
	FragPos = position + offset;
	gl_Position = PV * vec4(FragPos, 1.0f);
	ClipPos = gl_Position;
	TBN = mat3(
		tangent,
		bitangent,
		normal
	);
}