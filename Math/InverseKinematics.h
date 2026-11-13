#pragma once
#include <math.h>
struct InverseKinematicPoint 
{
	float originX;
	float originY;
	float originZ;
	float armLength;
};

inline void inverseKinematicIteration(InverseKinematicPoint* points, const float* target) 
{
	float reachX = target[0];
	float reachY = target[1];
	float reachZ = target[2];
	InverseKinematicPoint* pointIterator = points;
	while(pointIterator->armLength > 0.0f) 
	{
		pointIterator++;
	}
	while(pointIterator != points) 
	{
		float dx = reachX - pointIterator->originX;
		float dy = reachY - pointIterator->originY;
		float dz = reachZ - pointIterator->originZ;
		if(dx == 0.0f && dy == 0.0f && dz == 0.0f) 
		{
			dx = float((rand() % 256) - 128) / 2048.0f;
			dy = float((rand() % 256) - 128) / 2048.0f;
			dz = float((rand() % 256) - 128) / 2048.0f;	
			pointIterator->originX -= dx;
			pointIterator->originY -= dy;
			pointIterator->originZ -= dz;
		}
 		const float moveLength = 1.0f - pointIterator->armLength / std::sqrt(dx*dx+dy*dy+dz*dz);
		pointIterator->originX += dx * moveLength;
		pointIterator->originY += dy * moveLength;
		pointIterator->originZ += dz * moveLength;
		reachX = pointIterator->originX;
		reachY = pointIterator->originY;
		reachZ = pointIterator->originZ;
		pointIterator--;
	}
	pointIterator++;
	while(pointIterator->armLength > 0.0f) 
	{
		InverseKinematicPoint* armBase = pointIterator - 1;
		float dx = armBase->originX - pointIterator->originX;
		float dy = armBase->originY - pointIterator->originY;
		float dz = armBase->originZ - pointIterator->originZ;
		if(dx == 0.0f && dy == 0.0f && dz == 0.0f) 
		{
			dx = float((rand() % 256) - 128) / 2048.0f;
			dy = float((rand() % 256) - 128) / 2048.0f;
			dz = float((rand() % 256) - 128) / 2048.0f;	
			pointIterator->originX -= dx;
			pointIterator->originY -= dy;
			pointIterator->originZ -= dz;
		}
		const float moveLength = 1.0f - armBase->armLength / std::sqrt(dx*dx+dy*dy+dz*dz);
		pointIterator->originX += dx * moveLength;
		pointIterator->originY += dy * moveLength;
		pointIterator->originZ += dz * moveLength;
		pointIterator++;
	}
}