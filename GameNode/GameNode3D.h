#pragma once
#include "Transform.h"
#include "GameNode.h"
#include "Default/AABB.h"
#include "../EventSystem/EventListener.h"

using namespace glm;
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

		GameNode3D() 
		{
			setAABB(vec3(0.0f), vec3(1.0f));
		}

		virtual Transform getWorldTransform()
		{
			if (parrent) {
				if (dynamic_cast<GameNode3D*>(parrent))
				{
					Transform parrentTransform = ((GameNode3D*)parrent)->getWorldTransform();
					if (parrentLastTransform != parrentTransform || lastTransform != transform)
					{
						worldTransform = getTransform() * parrentTransform;
						worldTransform.getMatrix();
						worldTransform.getNormalsModel();
						parrentLastTransform = parrentTransform;
						lastTransform = transform;
					}
					return worldTransform;
				}
				else
					return transform;
			}
			else return transform;
		}

		virtual void setTransform(Transform transform)
		{
			if (dynamic_cast<GameNode3D*>(parrent))
				this->transform = transform / ((GameNode3D*)parrent)->getWorldTransform();
			else
				this->transform = transform;
		}

		virtual void addTransform(Transform transform)
		{
			this->transform *= transform;
		}

		virtual Transform getTransform() 
		{
			return transform;
		}

		virtual void setRelativeTransform(Transform transform)
		{
			this->transform = transform;
		}

		AABB getAABB(bool includeChildAABB) 
		{
			AABB aabb;
			aabb.position = this->transform.pointFromLocalToGlobal(this->aabb.position);
			aabb.scale = aabb.position;

			//finding max and min positions of each corners of BB 
			for (int i = 0; i < 2; i++) 
			{
				for (int j = 0; j < 2; j++)
				{
					for (int k = 1; k < 2; k++)
					{
						vec3 cornerPos = this->transform.pointFromLocalToGlobal(this->aabb.position + this->aabb.scale * vec3(i, j, k));
						aabb.position = vec3(glm::min(aabb.position.x, cornerPos.x), glm::min(aabb.position.y, cornerPos.y), glm::min(aabb.position.z, cornerPos.z));
						aabb.scale = vec3(glm::max(aabb.scale.x, cornerPos.x), glm::max(aabb.scale.y, cornerPos.y), glm::max(aabb.scale.z, cornerPos.z));
					}
				}
			}

			//
			if (includeChildAABB) 
			{
				for (GameNode* child : this->childNodes)
				{
					if (dynamic_cast<GameNode3D*>(child))
					{
						AABB childAABB = ((GameNode3D*)child)->getAABB(NODE_TREE_AABB);
						childAABB.scale += childAABB.position;
						aabb.position = vec3(glm::min(aabb.position.x, childAABB.position.x), glm::min(aabb.position.y, childAABB.position.y), glm::min(aabb.position.z, childAABB.position.z));
						aabb.scale = vec3(glm::max(aabb.scale.x, childAABB.scale.x), glm::max(aabb.scale.y, childAABB.scale.y), glm::max(aabb.scale.z, childAABB.scale.z));
					}
				}
			}
			//before next line aabb.scale is'nt actualy scale but a position of a right/top/front sides of an aabb
 			aabb.scale -= aabb.position;
			return aabb;
		}

		AABB getRelativeAABB() 
		{
			return aabb;
		}

		void setAABB(vec3 offset, vec3 scale) 
		{
			this->aabb = AABB(offset, scale);
		}
	};
}