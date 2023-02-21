#include "GameNode/ComponentProp.h"
#include "System/Debug.h"
namespace TealEngine 
{
	FloatVecProp::FloatVecProp(float* vecptr, int veclength) 
	{
		this->propptr = vecptr;
		this->length = veclength;
	}

	Json FloatVecProp::get() 
	{
		std::vector<float> v(this->length);
		for(int i = 0; i < this->length; i++) v[i] = ((float*)this->propptr)[i];
		return Json(v);
	}

	void FloatVecProp::set(const Json& json) 
	{
		std::vector<float> v = json.get<std::vector<float>>();
		int minLength = std::min((int)v.size(), this->length);
		for(int i = 0; i < minLength; i++) 
		{
			((float*)this->propptr)[i] = v[i];
		}
	}

	const char* vectorValueLabels[] = 
	{
		"X",
		"Y",
		"Z",
		"W"
	};

	bool FloatVecProp::display(const char* label) 
	{ 
		int edited = 0;
		if(length == 2)
			edited += ImGui::InputFloat2(label, (float*)this->propptr);
		else
		if(length == 3)
			edited += ImGui::InputFloat3(label, (float*)this->propptr);
		else
		if(length == 4)
			edited += ImGui::InputFloat4(label, (float*)this->propptr);
		else
		{
			for(int i = 0; i < this->length; i++) 
			{
				std::string vectorValueLabel;
				vectorValueLabel = (i >= 4) ? "A" + std::to_string(i) : vectorValueLabels[i];
				ImGui::PushID((std::string(label) + std::to_string(i)).c_str());
				edited += ImGui::InputFloat(vectorValueLabel.c_str(), (float*)this->propptr);
				ImGui::PopID();
			}
		}
		
		return edited;
	}
}