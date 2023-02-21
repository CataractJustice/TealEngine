#include "GameNode/ComponentProp.h"
namespace TealEngine 
{
	const char* colorValueLabels[] =  
	{
		"R", "G", "B", "A"
	};

	ColorProp::ColorProp(float* vecptr, int veclength) : FloatVecProp(vecptr, veclength)
	{

	}

	bool ColorProp::display(const char* label) 
	{ 
		int edited = 0;
		

		if(this->length == 3)
			edited += ImGui::ColorEdit3(label, (float*)this->propptr);
		else
		if(this->length == 4)
			edited += ImGui::ColorEdit4(label, (float*)this->propptr);
		return edited;
	}
}