#version 330 core
layout (location = 0) in vec3 position;

layout (std140) uniform CameraBlock
{
	mat4 PV;
	vec3 camDir;
    vec3 camPos;
};

layout (std140) uniform TransformBlock
{
	mat4 model;
	mat4 normalModel;
};

void main()
{
	vec4 p = PV * model * vec4(position, 1.0f);
	gl_Position = p;
}