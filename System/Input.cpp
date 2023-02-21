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
			glm::vec2 deltaMPos = glm::vec2(0.0f, 0.0f);
			double deltaSPos = 0.0;
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
				if (action == GLFW_PRESS)
				{
					MouseButtonPressEvent e = MouseButtonPressEvent(button, mod);
 					MouseButtonPress(&e);
				}
				if (action == GLFW_RELEASE)
				{
					MouseButtonReleaseEvent e = MouseButtonReleaseEvent(button, mod);
					MouseButtonRelease(&e);
				}
				mButton[button] = action != GLFW_RELEASE;
			}

			void wheelCallback(GLFWwindow* window, double xoffset, double yoffset) 
			{
				WheelScroll(new WheelScrollEvent(xoffset, yoffset));
				scrollPos += yoffset;
			}

			double getScrollPos() { return scrollPos; }

			glm::vec2 getDeltaMousePos() 
			{
				return deltaMPos;
			}
			double getDeltaScrollPos() 
			{
				return deltaSPos;
			}
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

				if (action == GLFW_PRESS)
				{
					KeyPressEvent e = KeyPressEvent(key, scancode, mod);
					KeyPress(&e);
				}
				if (action == GLFW_REPEAT) 
				{
					KeyRepeatEvent e = KeyRepeatEvent(key, scancode, mod);
					KeyPress(&e);
				}
				if (action == GLFW_RELEASE)
				{
					KeyReleaseEvent e = KeyReleaseEvent(key, scancode, mod);
					KeyPress(&e);
				}
				keyboardKey[key] = action != GLFW_RELEASE;
			}
		}

		void init() 
		{
			GLFWwindow* wptr = (GLFWwindow*)Graphics::window->gl_window_ptr_();
			glfwSetKeyCallback(wptr,Keyboard::keyCallback);
			glfwSetCursorPosCallback(wptr, Mouse::moveCallback);
			glfwSetMouseButtonCallback(wptr, Mouse::buttonCallback);
			glfwSetScrollCallback(wptr, Mouse::wheelCallback);
		}

		void inputUpdate() 
		{
			static glm::vec2 lastMousePos = Mouse::getMousePos();
			static double lastScrollPos = Mouse::getScrollPos();

			Mouse::deltaMPos = Mouse::getMousePos() - lastMousePos;
			Mouse::deltaSPos = Mouse::getScrollPos() - lastScrollPos;

			lastMousePos = Mouse::getMousePos();
			lastScrollPos = Mouse::getScrollPos();
		}
	}
}