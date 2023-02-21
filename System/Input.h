#pragma once
#include "Graphics/Graphics.h"
#include "libs/glm/vec2.hpp"
namespace TealEngine 
{
	namespace Input
	{
		namespace Mouse 
		{
			bool isButtonPressed(unsigned int button);
			glm::vec2 getMousePos();
			glm::vec2 getDeltaMousePos();
			double getDeltaScrollPos();
			double getScrollPos();
			extern EventPublisher WheelScroll, MouseMove, MouseButtonPress, MouseButtonRelease;
		}

		namespace Keyboard 
		{
			bool isKeyPressed(unsigned int key);
			extern EventPublisher KeyPress, keyRepeat, KeyRelease;
		}

		void inputUpdate();
		void init();
	}
}