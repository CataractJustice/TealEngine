#pragma once
#include "libs/glm/glm.hpp"

float linear2DGridInterpolation(glm::vec2 position, float a00, float a10, float a01, float a11);
float smooth2DGridInterpolation(glm::vec2 position, float a00, float a10, float a01, float a11);
float linear3DGridInterpolation(glm::vec3 position, float a000, float a100, float a010, float a110, float a001, float a101, float a011, float a111);
float smooth3DGridInterpolation(glm::vec3 position, float a000, float a100, float a010, float a110, float a001, float a101, float a011, float a111);