#include "Actions/IAction.h"
#include <vector>
namespace TealEngine 
{
	class ActionList 
	{
		private:
		std::vector<IAction*> actions;
		int currentActionIndex;
		int redoCap;
		public:
		ActionList();
		~ActionList();
		//pushes new action to actions list
		//removes all current possible redo actions
		//the destructor of ActionList will call the destructor of the pushed actions
		void pushAction(IAction* action);
		//undoes one action
		void undoAction();
		//redoes one action
		void redoAction();
	};
}