#include "GameNode/ComponentProp.h"
namespace TealEngine 
{
	bool BoolProp::display(const char* label) { return ImGui::Checkbox(label, (bool*)propptr); }
}