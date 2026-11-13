#version 330 core
layout (location = 0) out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D textureImage;

void main()
{
    FragColor = texture(textureImage, TexCoord);
}