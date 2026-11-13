#version 330 core
layout (location = 0) in uint position;
layout (std140) uniform CameraBlock
{
	mat4 PV;
	vec3 camDir;
	vec3 camPos;
};

uniform vec3 offset;

void main()
{
	const float inv8191 = 1.0 / 8191.0;

	float edgeX = float(position & 63u);
	float edgeY = float((position >> 6u) & 63u);
	uint axis  = (position >> 12u) & 3u;
	float foffset = float((position >> 14u) & 262143u) * inv8191;
	
	vec3 fpos = vec3(edgeX, edgeY, edgeY);
	fpos = mix(vec3(foffset, edgeX, edgeY), fpos, float(axis != 0u));
	fpos = mix(fpos, vec3(edgeX, foffset, edgeY), float(axis == 1u));
	fpos = mix(fpos, vec3(edgeX, edgeY, foffset), float(axis == 2u));
	gl_Position = PV * vec4(fpos + offset, 1.0f);
}