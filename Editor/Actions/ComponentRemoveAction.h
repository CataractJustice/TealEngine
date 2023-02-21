#pragma once
#include "ComponentAddAction.h"
namespace TealEngine
{
	class ComponentRemoveAction : public ComponentAddAction
	{
		public:
		virtual void redo() override;
		virtual void undo() override;
		using ComponentAddAction::ComponentAddAction;
	};
} // namespace TealEngine
