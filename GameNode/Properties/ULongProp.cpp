#include "GameNode/ComponentProp.h"
namespace TealEngine 
{
	bool ULongProp::display(const char* label) { return ImGui::DragInt(label, (int*)propptr); }
}