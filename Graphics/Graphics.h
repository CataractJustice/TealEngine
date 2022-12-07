#pragma once
#include "libs/glew/include/GL/glew.h"
#include "libs/glfw/include/GLFW/glfw3.h"
#include <string>
#include "EventSystem/EventPublisher.h"
#include "libs/glm/vec2.hpp"
#include "Window.h"
namespace TealEngine
{
	namespace Graphics
	{
		std::string initStatusStr(unsigned int code);
		unsigned int init(std::string windowName);
		void display();
		void terminate();
		extern Window* window;
	}
}