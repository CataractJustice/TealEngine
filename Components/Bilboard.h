#pragma once
#include "GameNode/GameNode3D.h"
#include "GameNode/Component.h"
#include "libs/glm/glm.hpp"
namespace TealEngine
{
	class Billboard : public Component
	{
		public:
		glm::vec3 up;
		bool flat;
		Billboard(const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f), bool flat = true);
		void update() override;
	};
} // namespace TealEngine
