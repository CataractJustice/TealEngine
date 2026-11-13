#version 330 core
layout (location = 0) out vec4 FragColor;

uniform vec4 color = vec4(1.0f);
in vec2 TexCoord;
uniform sampler2D image;
void main()
{
	vec4 imgc = texture(image, TexCoord);
    FragColor = mix(color, imgc, imgc.a);
}