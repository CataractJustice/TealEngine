#include "GameNode/ComponentProp.h"
namespace TealEngine 
{
	bool IntProp::display(const char* label) { return ImGui::DragInt(label, (int*)propptr); }
}