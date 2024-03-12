#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D textureImage;
uniform vec4 color = vec4(1.0f);

void main()
{
	if(texture(textureImage, vec2(TexCoord.x, 1.0f - TexCoord.y)).r < 0.1f) discard;
    FragColor = vec4(color.r, color.g, color.b, 1.0f);
}