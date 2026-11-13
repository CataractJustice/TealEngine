#version 410 core
layout (location = 0) in vec3 aPos;
uniform float time;
void main()
{
	gl_Position = vec4(aPos.x + cos(time + gl_InstanceID / 10000.0)*0.7, aPos.y + sin(time + gl_InstanceID / 10000.0)*0.7, aPos.z, 1.0);
}