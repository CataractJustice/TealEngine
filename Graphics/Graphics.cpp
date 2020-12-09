#pragma once
#define GLEW_STATIC
#include "Graphics.h"
#include "System/Clock.h"
#include "EventSystem/WindowEvents/WindowResizeEvent.h"

namespace TealEngine
{
	namespace Graphics
	{
		namespace Window 
		{
			GLFWwindow* window;
			EventPublisher WindowResize;
			const GLFWvidmode* primMonVidMode;
			GLuint windowWidth, windowHeight;
			
			GLuint getScreenWidth() { return primMonVidMode->width; }
			GLuint getScreenHeight() { return primMonVidMode->height; }

			GLuint getWindowWidth() { return windowWidth; }
			GLuint getWindowHeight() { return windowHeight; }

			void windowResizeCallback(GLFWwindow* window, int width, int height)
			{
				WindowResize(&WindowResizeEvent(width, height));
				windowWidth = width;
				windowHeight = height;
				glViewport(0, 0, width, height);
			}

			void windowCloseCallback()
			{

			}
		}

		std::string glInitStatusStr(unsigned int code)
		{
			std::string stats[] = { 
				"glInit is fine!\n",
				"glfw initialization failed.\n",
				"window creating is failed.\n",
				"glew initialization failed.\n" };
			return stats[code];
		}

		unsigned int glInit(std::string windowName)
		{
			if (!glfwInit())
				return 1;

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			Window::window = glfwCreateWindow(640, 480, windowName.c_str(), NULL, NULL);
			Window::windowWidth = 640;
			Window::windowHeight = 480;
			if (!Window::window)
			{
				glfwTerminate();
				return 2;
			}

			glfwMakeContextCurrent(Window::window);
			glfwSetWindowSizeCallback(Window::window, Window::windowResizeCallback);
			Window::primMonVidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glewExperimental = GL_TRUE;
			if (GLEW_OK != glewInit())
			{
				return 3;
			}

			glViewport(0, 0, 640, 480);
			

			return 0;
		}

		

		void display()
		{
			glfwSwapBuffers(Window::window);
			glfwPollEvents();
		}

		void glTerminate()
		{
			glfwTerminate();
		}
	}
}