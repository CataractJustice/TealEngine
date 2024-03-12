#pragma once
#include "GameNode/Component.h"

namespace TealEngine 
{
	class Collider : public Component
	{
	public:
		enum class ColliderType
		{
			AABC,
			AAPLANE,
			BOX,
			HEIGHTMAP,
			PLANE,
			SPHERE,
			SCALAR_FIELD
		};
		Collider();
		~Collider();
		virtual ColliderType getColliderType() = 0;
	};
}

