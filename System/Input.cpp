#include "Input.h"
#include <iostream>
#include "Core.h"
#include "GameNode/Component.h"
#include "GameNode/SceneNode.h"
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
			DefaultDisableCursorOption defaultCursorDisable;
			bool cursorDisable = false;
			bool cursorIsDisabled = false;
			glm::vec2 mousePos;
			glm::vec2 activeCursorMousePos;
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
				if(!getInputActive()) return;
				deltaMPoAccum += glm::vec2(xpos, ypos) - mousePos;
				mousePos = glm::vec2(xpos, ypos);
				if(!cursorDisable) activeCursorMousePos = mousePos;
			}

			void buttonCallback(GLFWwindow* window, int button, int action, int mod) 
			{
				if(!getInputActive()) return;
				if(button < 0 || button >= (sizeof(mButton)/sizeof(*mButton)))  return;
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
				if(!getInputActive()) return;
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
			
			void setDisableCursorDefault(DefaultDisableCursorOption option) 
			{
				defaultCursorDisable = option;
			}
			void setDisableCursorCurrent(bool disabled) 
			{
				cursorDisable = disabled;
			}
		}

		namespace Keyboard
		{
			Component* focusedCharInput = nullptr;

			Component* getFocusedCharInput() { return focusedCharInput; }
			void focusCharInput(Component* c) { focusedCharInput = c; }

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
				if(!getInputActive()) return;
				
				if (action == GLFW_PRESS)
				{
					if(key == GLFW_KEY_ESCAPE) focusedCharInput = nullptr;
					if(key == GLFW_KEY_F1) Core::uiSpace.showEditorUI = !Core::uiSpace.showEditorUI;
					if(focusedCharInput) focusedCharInput->onKeyPress(key); else Core::sceneNode()->onKeyPress(key);
				}
				if (action == GLFW_REPEAT) 
				{
					if(focusedCharInput) focusedCharInput->onKeyRepeat(key); else Core::sceneNode()->onKeyRepeat(key);
				}
				if (action == GLFW_RELEASE)
				{
					if(focusedCharInput) focusedCharInput->onKeyRelease(key); else Core::sceneNode()->onKeyRelease(key);
				}
				keyboardKey[key] = action != GLFW_RELEASE;
			}


			void charCallback(GLFWwindow* window, unsigned int unicode) 
			{
				if(focusedCharInput) 
				{
					focusedCharInput->onChar(unicode);
				}
				else 
				{
					Core::sceneNode()->onChar(unicode);
				}
			}
		}

		void init() 
		{
			GLFWwindow* wptr = (GLFWwindow*)Graphics::window->gl_window_ptr_();
			glfwSetKeyCallback(wptr,Keyboard::keyCallback);
			glfwSetCursorPosCallback(wptr, Mouse::moveCallback);
			glfwSetMouseButtonCallback(wptr, Mouse::buttonCallback);
			glfwSetScrollCallback(wptr, Mouse::wheelCallback);
			glfwSetCharCallback(wptr, Keyboard::charCallback);
		}

		void inputUpdate()
		{
			if(Core::uiSpace.showEditorUI) Mouse::cursorDisable = false;
			// Change cursor mode only if the desired mode changed since last frame
			if (Mouse::cursorIsDisabled != Mouse::cursorDisable)
			{
				glfwSetInputMode(
					(GLFWwindow*)Graphics::window->gl_window_ptr_(),
					GLFW_CURSOR,
					Mouse::cursorDisable ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
				);
				Mouse::mousePos = Mouse::activeCursorMousePos;
				Mouse::deltaMPoAccum = glm::vec2(0.0f);
				Mouse::cursorIsDisabled = Mouse::cursorDisable;
			}

			// Apply default cursor mode for this frame
			// Components can call setDisableCursorCurrent every frame to request cursor locking,
			// and when no component requires focus, the cursor returns to the default state.
			switch (Mouse::defaultCursorDisable)
			{
				case Mouse::DefaultDisableCursorOption::DisableOnFrameStart:
					Mouse::cursorDisable = true;
					break;
				case Mouse::DefaultDisableCursorOption::EnableOnFrameStart:
					Mouse::cursorDisable = false;
					break;
				case Mouse::DefaultDisableCursorOption::KeepValue:
				default:
					break;
			}

			if (!getInputActive()) return;

			static glm::vec2 lastMousePos = Mouse::getMousePos();
			static double	lastScrollPos = Mouse::getScrollPos();

			Mouse::deltaMPos	 = Mouse::deltaMPoAccum;
			Mouse::deltaMPoAccum = glm::vec2(0.0f);
			Mouse::deltaSPos	 = Mouse::getScrollPos() - lastScrollPos;

			lastMousePos  = Mouse::getMousePos();
			lastScrollPos = Mouse::getScrollPos();
		}
		void setInputActive(bool active) 
		{
			inputIsActive = active;
		}

		bool getInputActive() 
		{
			return inputIsActive && Core::getEngineState() == Core::EngineState::GAME_PLAYING;
		}
	}
}