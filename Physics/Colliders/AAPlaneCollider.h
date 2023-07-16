#pragma once
#include "Physics/Collider.h"

namespace TealEngine 
{	
	class AAPlaneCollider : public Collider 
	{
	public:

		enum class AAPCAxisAligment
		{
			XAXISPLANE,
			YAXISPLANE,
			ZAXISPLANE
		};

		AAPCAxisAligment aligment;
		bool negative;

		Collider::ColliderType getColliderType() 
		{
			return Collider::ColliderType::AAPLANE;
		}
	};
}