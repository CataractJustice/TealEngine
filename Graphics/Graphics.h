#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "EventSystem/EventPublisher.h"
namespace TealEngine
{
	namespace Graphics
	{
		std::string glInitStatusStr(unsigned int code);
		unsigned int glInit(std::string windowName);
		void display();
		void glTerminate();
		namespace Window
		{
			extern GLFWwindow* window;
			extern GLuint getScreenWidth();
			extern GLuint getScreenHeight();
			extern GLuint getWindowWidth();
			extern GLuint getWindowHeight();
			extern EventPublisher WindowResize;
		}
	}
}