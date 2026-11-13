#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D font;
uniform vec4 color;

void main()
{
	if(texture(font, vec2(TexCoord.x, TexCoord.y)).r < 0.5f) discard;
    FragColor = vec4(color.r, color.g, color.b, 1.0f);
}