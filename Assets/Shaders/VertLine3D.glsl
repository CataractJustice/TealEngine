#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

layout (std140) uniform CameraBlock
{
	mat4 PV;
	vec3 camDir;
	vec3 camPos;
};

uniform vec3 from;
uniform vec3 to;
uniform float thickness;

void main()
{
	vec4 screenPositionFrom = PV * vec4(from, 1.0f);
	vec4 screenPositionTo = PV * vec4(to, 1.0f);
	vec4 screenPosition = PV * vec4(mix(from, to, pos.x * 0.5f + 0.5f), 1.0f);
	vec2 normal = normalize(screenPositionTo.xy - screenPositionFrom.xy);
	normal = vec2(normal.y, -normal.x);
	screenPosition.xy += normal * pos.y * thickness;
	gl_Position = vec4(screenPosition.x, screenPosition.y, screenPosition.z, screenPosition.w);
}

