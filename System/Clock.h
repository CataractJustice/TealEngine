#pragma once
class Clock
{
private:
	double lastUpdate;
	double dTime;
public:

	Clock();
	float deltaTime();
	float getLastUpdate();
	void update();
	void reset();
	static double getTime();
};