#pragma once
#include <string>
#include "Shape.h"
#include "libs/glm/glm.hpp"

namespace TealEngine
{
	class Rect2D : public Shape
	{
		private:
			glm::vec2 offset;
			glm::vec2 scale;
			glm::vec4 color;
			std::string textureName;
			float thickness;
		public:
		void Draw() override;
		Rect2D(glm::vec2 offset, glm::vec2 scale, glm::vec4 color = glm::vec4(1.0f), const std::string& textureName = "");
	};
} // namespace TealEngine
