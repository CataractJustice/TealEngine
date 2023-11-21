#pragma once
#include "Graphics/Graphics.h"
#include "libs/glm/vec2.hpp"
namespace TealEngine 
{
	namespace Input
	{
		void setInputActive(bool active);
		bool getInputActive();
		namespace Mouse 
		{
			bool isButtonPressed(unsigned int button);
			glm::vec2 getMousePos();
			glm::vec2 getDeltaMousePos();
			double getDeltaScrollPos();
			double getScrollPos();
		}

		namespace Keyboard 
		{
			bool isKeyPressed(unsigned int key);
		}

		void inputUpdate();
		void init();
	}
}