#version 410 core
out vec4 FragColor;
uniform sampler2D tex;
uniform sampler2D tex2[2];
void main()
{
	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f) * texture(tex, vec2(0.5, 0.5));
} 