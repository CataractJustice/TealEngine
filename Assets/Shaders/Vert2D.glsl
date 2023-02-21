#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 3) in vec2 vertexUV;

out vec2 TexCoord;

uniform vec2 offset;
uniform vec2 scale = vec2(1.0f);

void main()
{
	TexCoord = vertexUV;
	gl_Position = vec4(vec3(vertexPosition.x * scale.x, vertexPosition.y * scale.y, vertexPosition.z) + vec3(offset, 0.0f), 1.0f);
}