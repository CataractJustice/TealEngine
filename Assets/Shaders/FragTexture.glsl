#version 330 core
layout (location = 0) out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D textureImage;
uniform vec4 color = vec4(1.0f);

void main()
{
    FragColor = texture(textureImage, vec2(TexCoord.x, TexCoord.y)) * color;
}