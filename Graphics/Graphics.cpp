#pragma once
#define GLEW_STATIC
#include "Graphics.h"
#include "System/Clock.h"
#include "EventSystem/WindowEvents/WindowResizeEvent.h"
#include "libs/glm/vec2.hpp"
#include "System/Debug.h"

namespace TealEngine
{
	namespace Graphics
	{
	Window* window = nullptr;
		

		std::string initStatusStr(unsigned int code)
		{
			std::string stats[] = { 
				"glInit is fine!\n",
				"glfw initialization failed.\n",
				"window creating is failed.\n",
				"glew initialization failed.\n" };
			return stats[code];
		}

		unsigned int init(std::string windowName)
		{
			TE_DEBUG_INFO("GLFW init.");
			if (!glfwInit())
				return 1;
			TE_DEBUG_INFO("Creating window.");
			window = new Window(windowName.c_str());
			window->setCurrent();
			glewExperimental = GL_TRUE;

			TE_DEBUG_INFO("GLEW init.");
			if (GLEW_OK != glewInit())
			{
				return 3;
			}

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  
			glEnable(GL_BLEND);

			return 0;
		}

		

		void display()
		{
			//TE_DEBUG_INFO("Swaping buffers.")
			glfwSwapBuffers((GLFWwindow*)window->gl_window_ptr_());

			//TE_DEBUG_INFO("Poll events.")
			glfwPollEvents();
		}

		void terminate()
		{
			glfwTerminate();
		}
	}
}