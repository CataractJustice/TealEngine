#include "GameNode/ComponentProp.h"
namespace TealEngine 
{
	bool FloatProp::display(const char* label) { return ImGui::DragFloat(label, (float*)propptr); }
}