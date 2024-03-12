#pragma once
#include "GameNode/Component.h"
namespace TealEngine 
{
	class BackgroundColor : public Component 
	{
		private:

		public:
		glm::vec4 color;
		glm::vec4 pressColor;
		BackgroundColor();
		void GUIRender() override;
	};
}