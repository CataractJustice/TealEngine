#pragma once
#include "Transform.h"
#include "GameNode.h"
#include "NlohmannJson/json.hpp"
#include "Properties/TransformProp.h"

using Json = nlohmann::json;

namespace TealEngine 
{
	class GameNode3D : public GameNode
	{
	private:
		mutable Transform lastWorldTransform, lastTransfrom, lastParentWorldTransform;
		mutable unsigned int transformLastModifyStamp, parentWorldTransformLastModifyStamp;
		bool hasGameNode3DParent;

		void setParent(GameNode* parent) override;

	protected:
		Transform transform;
	public:
		TransformProp* transformProp;

		GameNode3D();
		virtual ~GameNode3D();

		//do not try to modify game node world transform through this reference
		virtual const Transform& getWorldTransform() const;

		virtual void setWorldTransform(Transform transform);

		virtual void addTransform(const Transform& transform);

		virtual Transform& getRelativeTransform();

		virtual void setRelativeTransform(const Transform& transform);

		Json toJson() override;
		void displayProps() override;
	};
}