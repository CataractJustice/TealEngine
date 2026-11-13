#version 330 core
layout (location = 0) in vec3 position;
layout (location = 9) in mat4 model;

layout (std140) uniform CameraBlock
{
	mat4 PV;
	vec3 camDir;
    vec3 camPos;
};

void main()
{
	gl_Position = PV * model * vec4(position, 1.0f);
}