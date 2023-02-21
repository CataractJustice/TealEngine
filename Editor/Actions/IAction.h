#pragma once
namespace TealEngine
{
	class IAction 
	{
		private:

		public:
		virtual void undo() = 0;
		virtual void redo() = 0;
	};
}
