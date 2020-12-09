#include "Interpolation.h"

//Math and code here looks scary but all it's does is just interpolate betwen values
//1's and 0's in argument names are corner indexes

float linear2DGridInterpolation(glm::vec2 position, float a00, float a10, float a01, float a11)
{
	glm::vec2 f = glm::fract(position);
	return (a00 * (1.0f - f.x) + a10 * f.x) * (1.0f - f.y) + (a01 * (1.0f - f.x) + a11 * f.x) * f.y;
}

float smooth2DGridInterpolation(glm::vec2 position, float a00, float a10, float a01, float a11)
{
	glm::vec2 f = glm::smoothstep(glm::vec2(0.0f), glm::vec2(1.0f), glm::fract(position));
	return (a00 * (1.0f - f.x) + a10 * f.x) * (1.0f - f.y) + (a01 * (1.0f - f.x) + a11 * f.x) * f.y;
}

float linear3DGridInterpolation(glm::vec3 position, float a000, float a100, float a010, float a110, float a001, float a101, float a011, float a111)
{
	glm::vec3 f = glm::fract(position);
	return ((a000 * (1.0f - f.x) + a100 * f.x) * (1.0f - f.y) + (a010 * (1.0f - f.x) + a110 * f.x) * f.y) * (1.0f - f.z) + ((a001 * (1.0f - f.x) + a101 * f.x) * (1.0f - f.y) + (a011 * (1.0f - f.x) + a111 * f.x) * f.y) * f.z;
}

float smooth3DGridInterpolation(glm::vec3 position, float a000, float a100, float a010, float a110, float a001, float a101, float a011, float a111)
{
	glm::vec3 f = glm::smoothstep(glm::vec3(0.0f), glm::vec3(1.0f), glm::fract(position));
	return ((a000 * (1.0f - f.x) + a100 * f.x) * (1.0f - f.y) + (a010 * (1.0f - f.x) + a110 * f.x) * f.y) * (1.0f - f.z) + ((a001 * (1.0f - f.x) + a101 * f.x) * (1.0f - f.y) + (a011 * (1.0f - f.x) + a111 * f.x) * f.y) * f.z;
}