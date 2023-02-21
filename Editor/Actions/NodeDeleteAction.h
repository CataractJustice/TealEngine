#pragma once
#include "NodeCreateAction.h"

namespace TealEngine
{
	class NodeDeleteAction : public NodeCreateAction
	{
		public:
		virtual void redo() override;
		virtual void undo() override;
		using NodeCreateAction::NodeCreateAction;
	};
} // namespace TealEngine
