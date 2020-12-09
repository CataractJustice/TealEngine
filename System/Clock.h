#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>
class Clock
{
private:
	double lastUpdate;
	double dTime;
public:

	Clock() 
	{
		lastUpdate = dTime = 0.0f;
	}

	GLfloat deltaTime() 
	{
		return dTime;
	}

	GLfloat getLastUpdate() 
	{
		return lastUpdate;
	}

	void update() 
	{
		dTime = glfwGetTime() - lastUpdate;
		lastUpdate = lastUpdate + dTime;
	}

	void reset() 
	{
		dTime = 0.0f;
		lastUpdate = 0.0f;
	}

	static double getTime() 
	{
		return glfwGetTime();
	}
};