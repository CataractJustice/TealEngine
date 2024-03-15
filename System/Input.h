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
			//returns window mouse cursor position in pixels
			glm::vec2 getMousePos();
			//returns change of mouse cursor position between two frames in pixels 
			glm::vec2 getDeltaMousePos();
			//returns cnage of scroll position between two frames
			double getDeltaScrollPos();
			double getScrollPos();
			void setCursorHidden(bool hidden);
			void setCursorPosition(int px, int py);
		}

		namespace Keyboard 
		{
			bool isKeyPressed(unsigned int key);
		}

		void inputUpdate();
		void init();
	}
}