#include "Interpolation.h"

//Math and code here looks scary but all it's does is just interpolate betwen values
//1's and 0's in argument names are corner indexes

float linear2DGridInterpolation(const glm::vec2& position, const float& a00, const float& a10, const float& a01, const float& a11)
{
	return (a00 * (1.0f - position.x) + a10 * position.x) * (1.0f - position.y) + (a01 * (1.0f - position.x) + a11 * position.x) * position.y;
}

float smooth2DGridInterpolation(const glm::vec2& position, const float& a00, const float& a10, const float& a01, const float& a11)
{
	glm::vec2 f = glm::smoothstep(glm::vec2(0.0f), glm::vec2(1.0f), position);
	return (a00 * (1.0f - f.x) + a10 * f.x) * (1.0f - f.y) + (a01 * (1.0f - f.x) + a11 * f.x) * f.y;
}

float linear3DGridInterpolation(const glm::vec3& position, const float& a000, const float& a100, const float& a010, const float& a110, const float& a001, const float& a101, const float& a011, const float& a111)
{
	return ((a000 * (1.0f - position.x) + a100 * position.x) * (1.0f - position.y) + (a010 * (1.0f - position.x) + a110 * position.x) * position.y) * (1.0f - position.z) + ((a001 * (1.0f - position.x) + a101 * position.x) * (1.0f - position.y) + (a011 * (1.0f - position.x) + a111 * position.x) * position.y) * position.z;
}

float smooth3DGridInterpolation(const glm::vec3& position, const float& a000, const float& a100, const float& a010, const float& a110, const float& a001, const float& a101, const float& a011, const float& a111)
{
	glm::vec3 f = glm::smoothstep(glm::vec3(0.0f), glm::vec3(1.0f), position);
	return ((a000 * (1.0f - f.x) + a100 * f.x) * (1.0f - f.y) + (a010 * (1.0f - f.x) + a110 * f.x) * f.y) * (1.0f - f.z) + ((a001 * (1.0f - f.x) + a101 * f.x) * (1.0f - f.y) + (a011 * (1.0f - f.x) + a111 * f.x) * f.y) * f.z;
}