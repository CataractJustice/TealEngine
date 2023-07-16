#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 3) in vec2 vertexUV;

out vec2 TexCoord;

uniform mat4 pv_mat;
uniform vec3 from;
uniform vec3 to;
uniform float thickness;
uniform mat4 model;

void main()
{
	TexCoord = vertexUV;
	vec4 screenPositionFrom = pv_mat * vec4(from, 1.0f);
	vec4 screenPositionTo = pv_mat * vec4(to, 1.0f);
	vec4 screenPosition = pv_mat * vec4(mix(from, to, vertexPosition.x * 0.5f + 0.5f), 1.0f);
	vec2 normal = normalize(screenPositionTo.xy - screenPositionFrom.xy);
	normal = vec2(normal.y, -normal.x);
	screenPosition.xy += normal * vertexPosition.y * thickness;//	 * screenPosition.w;
	gl_Position = vec4(screenPosition.x, screenPosition.y, screenPosition.z, screenPosition.w);//vec4(screenPosition.x + normal.x * thickness * vertexPosition.y/2.0f, screenPosition.y + normal.y * thickness * vertexPosition.y/2.0f, screenPosition.z, 1.0f);
}