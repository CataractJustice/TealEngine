#include "GameNode/ComponentProp.h"

namespace TealEngine 
{
	EnumProp::EnumProp(int* enumptr, const char** enumNames, int enumCount) : enumNames(enumNames), enumCount(enumCount), TProp(enumptr)
	{

	}

	bool EnumProp::display(const char* label) 
	{
		return ImGui::Combo(label, (int*)this->propptr, this->enumNames, this->enumCount);
	} 
}