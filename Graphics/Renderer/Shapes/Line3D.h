#pragma once

#include "Shape.h"
#include "libs/glm/glm.hpp"

namespace TealEngine
{
	class Line3D : public Shape
	{
		private:
			glm::vec3 from;
			glm::vec3 to;
			glm::vec4 color;
			float thickness;
		public:
		void Draw() override;
		Line3D(glm::vec3 from, glm::vec3 to, glm::vec4 color = glm::vec4(1.0f), float thickness = 0.1f);
	};
} // namespace TealEngine
