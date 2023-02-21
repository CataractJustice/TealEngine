#pragma once
#include "IAction.h"
#include "NlohmannJson/json.hpp"
#include "GameNode/GameNode.h"
using Json = nlohmann::json;

namespace TealEngine
{
	class ComponentAddAction : public IAction
	{
		Json componentJson;
		Component* component;
		int nodeId;
		public:
		virtual void redo() override;
		virtual void undo() override;
		ComponentAddAction(Component* component);
	};
} // namespace TealEngine
