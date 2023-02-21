#include "ComponentRemoveAction.h"

namespace TealEngine
{
	void ComponentRemoveAction::redo() 
	{
		ComponentAddAction::undo();
	}

	void ComponentRemoveAction::undo() 
	{
		ComponentAddAction::redo();
	}
} // namespace TealEngine
