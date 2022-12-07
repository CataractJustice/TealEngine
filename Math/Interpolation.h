#pragma once
#include "libs/glm/glm.hpp"

float linear2DGridInterpolation(const glm::vec2& position, const float& a00,  const float& a10,  const float& a01,  const float& a11);
float smooth2DGridInterpolation(const glm::vec2& position, const float& a00,  const float& a10,  const float& a01,  const float& a11);
float linear3DGridInterpolation(const glm::vec3& position, const float& a000, const float& a100, const float& a010, const float& a110, const float& a001, const float& a101, const float& a011, const float& a111);
float smooth3DGridInterpolation(const glm::vec3& position, const float& a000, const float& a100, const float& a010, const float& a110, const float& a001, const float& a101, const float& a011, const float& a111);