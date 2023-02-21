#include "GameNode/ComponentProp.h"
namespace TealEngine 
{
	bool UShortProp::display(const char* label) { return ImGui::DragInt(label, (int*)propptr); }
}