#pragma once
#include "GameNode/Component.h"
namespace TealEngine 
{
	class SolidBorder : public Component 
	{
		private:

		public:
		glm::vec4 color;
		SolidBorder();
		void GUIRender() override;
	};
}