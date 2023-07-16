#include "GameNode/ComponentProp.h"
namespace TealEngine 
{
	bool BoolProp::display(const char* label) 
	{ 
		ImGui::PushID(label);
		bool change = ImGui::Checkbox(label, (bool*)propptr); 
		ImGui::PopID();
		return change;
	}
}