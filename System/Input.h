#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>
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
			double getScrollPos();
			extern EventPublisher WheelScroll, MouseMove, MouseButtonPress, MouseButtonRelease;
		}

		namespace Keyboard 
		{
			bool isKeyPressed(unsigned int key);
			extern EventPublisher KeyPress, keyRepeat, KeyRelease;
		}

		void init();
	}
}