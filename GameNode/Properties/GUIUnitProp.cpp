#include "GUIUnitProp.h"
#include "libs/glm/gtc/type_ptr.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"
#include <cmath>
#include <cctype>

void parseUnitsString(const char* str, float& px, float& ww, float& wh, float& rw, float& rh, float& pw, float& ph) 
{
	px = ww = wh = rw = rh = pw = ph = 0.0f;
	int val = 0;
	int valLen = 0;
	int valPoint = 0;
	bool hasPoint = false;
	bool negative = false;

	while (*str) 
	{
		if (std::isdigit(*str)) 
		{
			val *= 10;
			val += *str - '0';  
			valLen++;
		} 
		else if(*str == '-') 
		{
			negative = true;
		}
		else if (*str == '.' || *str == ',') 
		{
			valPoint = valLen;
			hasPoint = true;
		}
		else if (val != 0) 
		{
			float fval = hasPoint ? static_cast<float>(val) / std::pow(10.0f, valLen - valPoint) : static_cast<float>(val);
			if(negative) fval = -fval;
			switch (*str) 
			{
				case 'p':
					switch (*(str + 1)) {
						case 'x': px += fval; break;
						case 'w': pw += fval; break;
						case 'h': ph += fval; break;
					}
					break;
				case 'w':
					switch (*(str + 1)) {
						case 'w': ww += fval; break;
						case 'h': wh += fval; break;
					}
					break;
				case 'r':
					switch (*(str + 1)) {
						case 'w': rw += fval; break;
						case 'h': rh += fval; break;
					}
					break;
				default:
					break;
			}
			val = 0;
			valLen = 0;
			valPoint = 0;
			hasPoint = false;
			negative = false;
			//Here we assume that all unit suffics are at least 2 chars long
			str++;
		}
		str++;
	}
}

namespace TealEngine 
{
	GUIUnitProp::GUIUnitProp(GUIUnit* unitPtr)
	{
		this->propptr = unitPtr;
	}
	
	Json GUIUnitProp::get()
	{
		GUIUnit& unit = this->valueref<GUIUnit>();
		Json json;
		float px = unit.getPixelsValue();
		float ww = unit.getWindowWidthsValue();
		float wh = unit.getWindowHeightsValue();
		float rw = unit.getRootWidthsValue();
		float rh = unit.getRootHeightsValue();
		float pw = unit.getParentWidthsValue();
		float ph = unit.getParentHeightsValue();
		json = 
		(px ? std::to_string(px) + "px" : "") + 
		(ww ? std::to_string(ww) + "ww" : "") + 
		(wh ? std::to_string(wh) + "wh" : "") + 
		(rw ? std::to_string(rw) + "rw" : "") + 
		(rh ? std::to_string(rh) + "rh" : "") + 
		(pw ? std::to_string(pw) + "pw" : "") + 
		(ph ? std::to_string(ph) + "ph" : "")
		;
		return json;
	}
	void GUIUnitProp::set(const Json& json) 
	{
		GUIUnit& unit = this->valueref<GUIUnit>();
		parseUnitsString(json.get<std::string>().c_str(),
			unit.getPixelsValue(),
			unit.getWindowWidthsValue(),
			unit.getWindowHeightsValue(),
			unit.getRootWidthsValue(),
			unit.getRootHeightsValue(),
			unit.getParentWidthsValue(),
			unit.getParentHeightsValue()
		);
	}
	bool GUIUnitProp::display(const char* label) 
	{
		bool edited = ImGui::InputText(label, &str);
		if(!ImGui::IsItemFocused()) 
		{
			str = get().get<std::string>();
		}
		if(edited) set(str);
		return edited;
	}
}