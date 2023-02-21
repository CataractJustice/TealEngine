#include "GameNode/ComponentProp.h"
namespace TealEngine 
{
	bool StringProp::display(const char* label) { return ImGui::InputText(label, &valueref<std::string>()); }
}