#pragma once
#include "Transform.h"
#include "GameNode.h"
#include "EventSystem/EventListener.h"
#include "NlohmannJson/json.hpp"
#include "Properties/TransformProp.h"

using Json = nlohmann::json;

namespace TealEngine 
{
	class GameNode3D : public GameNode
	{
	private:
		Transform lastWorldTransform, lastTransfrom, lastParentWorldTransform;
		unsigned int transformLastModifyStamp, parentWorldTransformLastModifyStamp;
		bool hasGameNode3DParent;

		void setParent(GameNode* parent) override;
		TransformProp* transformProp;

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

		static GameNode3D* nodeFromJson(const Json& json);
		static GameNode3D* loadNodeFromJsonFile(const std::filesystem::path& path);
		Json toJson() override;
		void displayProps() override;
	};
}