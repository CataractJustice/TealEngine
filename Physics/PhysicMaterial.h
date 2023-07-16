#pragma once
namespace TealEngine 
{
	class PhysicMaterial 
	{
	public:
		float mass;
		float bouncines;
		float friction;

		PhysicMaterial() : mass(1.0f), bouncines(0.3f), friction(15.0f) {}
	};
}