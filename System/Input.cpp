#include "Input.h"
#include "EventSystem/KeyboardEvents/KeyboardEvents.h"
#include "EventSystem/MouseEvents/MouseEvents.h"
#include <iostream>
namespace TealEngine
{
	namespace Input
	{
		namespace Mouse
		{
			EventPublisher WheelScroll, MouseMove, MouseButtonPress, MouseButtonRelease;
			bool mButton[3] = {0};
			double scrollPos = 0;
			glm::vec2 mousePos;
			bool isButtonPressed(unsigned int button) 
			{
				if (button <= 3)
					return mButton[button];
				else
					return false;
			}
			glm::vec2 getMousePos() 
			{
				return mousePos;
			}

			void moveCallback(GLFWwindow* window, double xpos, double ypos) 
			{
				MouseMove(new MouseMoveEvent(xpos, ypos, xpos - mousePos.x, ypos - mousePos.y));
				mousePos = glm::vec2(xpos, ypos);
			}

			void buttonCallback(GLFWwindow* window, int button, int action, int mod) 
			{
				if (action == GLFW_PRESS) MouseButtonPress(&MouseButtonPressEvent(button, mod));
				if (action == GLFW_RELEASE) MouseButtonRelease(&MouseButtonReleaseEvent(button, mod));
				mButton[button] = action != GLFW_RELEASE;
			}

			void wheelCallback(GLFWwindow* window, double xoffset, double yoffset) 
			{
				WheelScroll(new WheelScrollEvent(xoffset, yoffset));
				scrollPos += yoffset;
			}

			double getScrollPos() { return scrollPos; }
		}

		namespace Keyboard
		{
			EventPublisher KeyPress, keyRepeat, KeyRelease;
			bool keyboardKey[1024] = {0};
			bool isKeyPressed(unsigned int key) 
			{
				if (key < 1024)
					return keyboardKey[key];
				else
					return false;
			}
		
			void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mod) 
			{
				if (action == GLFW_PRESS) KeyPress(&KeyPressEvent(key, scancode, mod));
				if (action == GLFW_REPEAT) KeyPress(&KeyRepeatEvent(key, scancode, mod));
				if (action == GLFW_RELEASE) KeyPress(&KeyReleaseEvent(key, scancode, mod));
				keyboardKey[key] = action != GLFW_RELEASE;
			}
		}

		void init() 
		{
			glfwSetKeyCallback(Graphics::Window::window,Keyboard::keyCallback);
			glfwSetCursorPosCallback(Graphics::Window::window, Mouse::moveCallback);
			glfwSetMouseButtonCallback(Graphics::Window::window, Mouse::buttonCallback);
			glfwSetScrollCallback(Graphics::Window::window, Mouse::wheelCallback);
		}
	}
}