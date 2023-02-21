#include "GameNode/ComponentProp.h"
namespace TealEngine 
{
	bool CharProp::display(const char* label) { return ImGui::DragInt(label, (int*)propptr, 1.0f, 0, 255); }
}