#include "GameNode/ComponentProp.h"
namespace TealEngine 
{
	bool LongProp::display(const char* label) { return ImGui::DragInt(label, (int*)propptr); }
}