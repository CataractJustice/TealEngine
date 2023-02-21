#include "ActionList.h"

namespace TealEngine 
{
	ActionList::ActionList() : 
	actions(0), 
	currentActionIndex(0), 
	redoCap(0) 
	{};
	ActionList::~ActionList() 
	{
		for(IAction* action : actions) 
		{
			delete action;
		}		
	}

	void ActionList::pushAction(IAction* action) 
	{
		while(currentActionIndex < actions.size()) 
		{
			delete actions.back();
			actions.pop_back();
		}
		this->actions.push_back(action);
		this->currentActionIndex = actions.size();
		this->redoCap = actions.size();
	}

	void ActionList::undoAction() 
	{
		if(currentActionIndex) 
		{
			actions[--currentActionIndex]->undo();
		}
	}

	void ActionList::redoAction() 
	{
		if(currentActionIndex < redoCap) 
		{
			actions[currentActionIndex++]->redo();
		}
	}
}