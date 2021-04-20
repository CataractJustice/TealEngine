#pragma once
#include "Transform.h"
#include "GameNode.h"
#include "Default/AABB.h"
#include "EventSystem/EventListener.h"

namespace TealEngine 
{
#define NODE_TREE_AABB true
#define SINGLE_NODE_AABB false
	class GameNode3D : public GameNode
	{
	private:
		Transform worldTransform;
		Transform parrentLastTransform;
		Transform lastTransform;
	protected:
		AABB aabb;
		Transform transform;
	public:

		GameNode3D();

		virtual Transform getWorldTransform();

		virtual void setTransform(Transform transform);

		virtual void addTransform(Transform transform);

		virtual Transform& getTransform();

		virtual void setRelativeTransform(Transform transform);

		AABB getAABB(bool includeChildAABB);

		AABB getRelativeAABB();

		void setAABB(glm::vec3 offset, glm::vec3 scale);
	};
}