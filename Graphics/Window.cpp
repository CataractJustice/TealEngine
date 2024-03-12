#include "Window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <map>
#include <iostream>
namespace TealEngine 
{

	std::map<void*, Window*> glfwWindowRelations = std::map<void*, Window*>();
	Window* findByGLFWwindow(void* glfwW) { return glfwWindowRelations[glfwW]; }

	void windowResizeCallback(GLFWwindow* window, int width, int height)
	{
		Window* ourWindow = findByGLFWwindow(window);
		ourWindow->resize(width, height);
		glViewport(0, 0, width, height);
	}

	void printGlfwError(int code, const char* error) 
	{
		std::cout << "GG Glfw Issue: " << code << " " << error << "\n";
	}

	Window::Window(const char* title, int width, int height) 
	{
		glfwSetErrorCallback(&printGlfwError);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		Window::window = glfwCreateWindow(width, height, title, NULL, NULL);
		Window::windowWidth = width;
		Window::windowHeight = height;
		glfwSetWindowSizeCallback((GLFWwindow*)window, windowResizeCallback);
		primMonVidMode = (void*)glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwWindowRelations[window] = this;
	}

	void Window::resize(int w, int h) 
	{
		windowWidth = w;
		windowHeight = h;
	}

	unsigned int Window::getScreenWidth() { return ((GLFWvidmode*)(primMonVidMode))->width; }
	unsigned int Window::getScreenHeight() { return ((GLFWvidmode*)(primMonVidMode))->height; }
	unsigned int Window::getWindowWidth() { return windowWidth; }
	unsigned int Window::getWindowHeight() { return windowHeight; }
	float Window::getAspect() 
	{
		return ((float)windowWidth) / ((float)windowHeight);
	}

	glm::vec2 Window::getScreenResolution() { return glm::vec2(getScreenWidth(), getScreenHeight()); }
	glm::vec2 Window::getWindowResolution() { return glm::vec2(windowWidth, windowHeight); }
	void Window::setCurrent() 
	{
		glfwMakeContextCurrent((GLFWwindow*)window);
		glViewport(0, 0, windowWidth, windowHeight);
	}

	bool Window::shouldClose() 
	{
		return glfwWindowShouldClose((GLFWwindow*)window);
	}
	
	void* Window::gl_window_ptr_() { return window; }
	void* Window::prim_mon_mode_ptr_() { return primMonVidMode; }
}