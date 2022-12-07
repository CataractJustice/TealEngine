#pragma once
#include "Transform.h"
#include "GameNode.h"
#include "EventSystem/EventListener.h"

namespace TealEngine 
{
	class GameNode3D : public GameNode
	{
	private:
		Transform lastWorldTransform, lastTransfrom, lastParrentWorldTransform;
		unsigned int transformLastModifyStamp, parrentWorldTransformLastModifyStamp;
		bool hasGameNode3DParrent;

		void setParrent(GameNode* parrent) override;

	protected:
		Transform transform;
	public:

		GameNode3D();
		~GameNode3D();

		//do not try to modify game node world transform through this reference
		virtual Transform& getWorldTransform();

		virtual void setWorldTransform(Transform transform);

		virtual void addTransform(const Transform& transform);

		virtual Transform& getRelativeTransform();

		virtual void setRelativeTransform(const Transform& transform);

		void displayNodeTree(bool windowBegin) override;
	};
}