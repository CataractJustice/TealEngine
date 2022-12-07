#include "Clock.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

Clock::Clock()
{
	lastUpdate = glfwGetTime();
	dTime = 0.0f;
}

float Clock::deltaTime()
{
	return dTime;
}

float Clock::getLastUpdate()
{
	return lastUpdate;
}

void Clock::update()
{
	dTime = glfwGetTime() - lastUpdate;
	lastUpdate = lastUpdate + dTime;
}

void Clock::reset()
{
	dTime = 0.0f;
	lastUpdate = glfwGetTime();
}

double Clock::getTime()
{
	return glfwGetTime();
}