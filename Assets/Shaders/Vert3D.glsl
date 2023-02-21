#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 3) in vec2 vertexUV;

out vec2 TexCoord;

uniform mat4 pv_mat;
uniform mat4 model;

void main()
{
	TexCoord = vertexUV;
	gl_Position = pv_mat * model * vec4(vertexPosition, 1.0f);
}