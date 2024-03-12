#version 330 core
layout (location = 0) out vec4 FragColor;

uniform vec4 color = vec4(1.0f);

void main()
{
    FragColor = color;
}