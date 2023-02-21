#pragma once
#include "IAction.h"
#include "NlohmannJson/json.hpp"
#include "GameNode/GameNode.h"

using Json = nlohmann::json;

namespace TealEngine
{
	class NodeCreateAction : public IAction
	{
		Json nodeJson;
		GameNode* node;
		int parentId;
		public:
		virtual void redo() override;
		virtual void undo() override;
		NodeCreateAction(GameNode* node);
	};
} // namespace TealEngine
