#include "Input.h"
#include <iostream>
#include "Core.h"
#include "GameNode/Component.h"
namespace TealEngine
{
	namespace Input
	{
		bool inputIsActive = true;
		namespace Mouse
		{
			glm::vec2 deltaMPos = glm::vec2(0.0f, 0.0f);
			glm::vec2 deltaMPoAccum = glm::vec2(0.0f, 0.0f);
			double deltaSPos = 0.0;
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
				if(!inputIsActive) return;
				deltaMPoAccum += glm::vec2(xpos, ypos) - mousePos;
				mousePos = glm::vec2(xpos, ypos);
			}

			void buttonCallback(GLFWwindow* window, int button, int action, int mod) 
			{
				if(!inputIsActive) return;
				if (action == GLFW_PRESS)
				{	
					if(Core::getRoot()) 
					{
						Core::getRoot()->onWindowMousePress(button);
					}
				}
				if (action == GLFW_RELEASE)
				{	
					if(Core::getRoot()) 
					{
						Core::getRoot()->onWindowMouseRelease(button);
					}
				}
				mButton[button] = action != GLFW_RELEASE;
			}

			void wheelCallback(GLFWwindow* window, double xoffset, double yoffset) 
			{
				if(!inputIsActive) return;
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

			void setCursorPosition(int px, int py) 
			{
				glfwSetCursorPos((GLFWwindow*)Graphics::window->gl_window_ptr_(), px, py);
				mousePos = glm::vec2(px, py);
			}

			void setCursorHidden(bool hidden) 
			{
				if(hidden)
					glfwSetInputMode((GLFWwindow*)Graphics::window->gl_window_ptr_(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				else 
				{
					glfwSetInputMode((GLFWwindow*)Graphics::window->gl_window_ptr_(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				}
			}
		}

		namespace Keyboard
		{
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
				if(!inputIsActive) return;
				if (action == GLFW_PRESS)
				{
					if(key == GLFW_KEY_F1) Core::uiSpace.showEditorUI = !Core::uiSpace.showEditorUI;
					Component::onKeyPressCallbacks(key);
				}
				if (action == GLFW_REPEAT) 
				{
				}
				if (action == GLFW_RELEASE)
				{
					Component::onKeyReleaseCallbacks(key);
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
			if(!inputIsActive) return;
			static glm::vec2 lastMousePos = Mouse::getMousePos();
			static double lastScrollPos = Mouse::getScrollPos();

			Mouse::deltaMPos = Mouse::deltaMPoAccum;
			Mouse::deltaMPoAccum = glm::vec2(0.0f);
			Mouse::deltaSPos = Mouse::getScrollPos() - lastScrollPos;

			lastMousePos = Mouse::getMousePos();
			lastScrollPos = Mouse::getScrollPos();
		}
		void setInputActive(bool active) 
		{
			inputIsActive = active;
		}

		bool getInputActive() 
		{
			return inputIsActive;
		}
	}
}