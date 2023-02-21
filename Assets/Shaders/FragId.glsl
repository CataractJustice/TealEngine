/*

*/

#version 330 core

layout (location = 0) out int FragId;

uniform int id;

void main()
{
    FragId = id;
}