#include "TreeDisplay.h"
#include "Editor/UI/UISpace.h"

namespace TealEngine 
{
	TreeDisplay::TreeDisplay(const std::string& windowName) :
	windowName(windowName)
	{

	}
	void TreeDisplay::imGuiRender(const std::string& windowName) 
	{
		if(windowName == this->windowName) 
		{
			this->getParent()->displayNodeTree();
		}
	}
}