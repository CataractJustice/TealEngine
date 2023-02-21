#pragma once
#include "IAction.h"
#include "NlohmannJson/json.hpp"
#include "GameNode/Component.h"
using Json = nlohmann::json;
namespace TealEngine
{
	class PropSetAction : public IAction
	{
		Json oldValue;
		Json newValue;
		int componentId;	
		public:

		virtual void redo() override;
		virtual void undo() override;
		PropSetAction(Component* component, const Json& oldValue, const Json& newValue);
	};
} // namespace TealEngine
