#version 460 core
layout (location = 5) out vec4 LightColor;

in vec2 TexCoord;

uniform sampler2D textureImage;

void main()
{
    LightColor = texture(textureImage, vec2(TexCoord.x, TexCoord.y));
}