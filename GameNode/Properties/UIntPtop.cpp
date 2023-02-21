#include "GameNode/ComponentProp.h"
namespace TealEngine 
{
	bool UIntProp::display(const char* label) { return ImGui::DragInt(label, (int*)propptr); }
}