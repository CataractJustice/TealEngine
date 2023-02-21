#include "NodeDeleteAction.h"

namespace TealEngine 
{
	void NodeDeleteAction::redo() 
	{
		NodeCreateAction::undo();
	}

	void NodeDeleteAction::undo() 
	{
		NodeCreateAction::redo();
	}
}