//////////void fn ptr
#include "GameNode/ComponentProp.h"
namespace TealEngine 
{
	void VoidFunctionProp::set(const Json& json)
	{
		return;
	}
	
	Json VoidFunctionProp::get()
	{
		return Json("");
	}
	
	inline bool VoidFunctionProp::display(const char* label)
	{
		if(ImGui::Button(label)) 
		{
			this->valueref<std::function<void()>>()();
			return true;
		}
		return false;
	}
}