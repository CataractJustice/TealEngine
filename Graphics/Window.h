#pragma once
#include "libs/glm/vec2.hpp"
#include "EventSystem/EventPublisher.h"
namespace TealEngine 
{
	class Window
	{
	private:
		void* window;
		void* primMonVidMode;
		unsigned int windowWidth, windowHeight;
	public:

		Window(const char* title, int width = 640, int height = 480);

		void resize(int w, int h);

		unsigned int getScreenWidth();
		unsigned int getScreenHeight();
		unsigned int getWindowWidth();
		unsigned int getWindowHeight();
		float getAspect();
		void setCurrent();
		glm::vec2 getScreenResolution();
		glm::vec2 getWindowResolution();
		EventPublisher WindowResize;
		bool shouldClose();

		void* gl_window_ptr_();
		void* prim_mon_mode_ptr_();
	};
}