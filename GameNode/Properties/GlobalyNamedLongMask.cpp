#include "GlobalyNamedLongMask.h"
#include "GameNode/Component.h"
#include <format>
namespace TealEngine 
{
	GlobalyNamedLongMask::GlobalyNamedLongMask(unsigned long long* maskPtr, std::string* names)
	{
		this->propptr = maskPtr;
		this->names = names;
	}
	
	Json GlobalyNamedLongMask::get() 
	{
		Json json;
		json["mask"] = this->valueref<unsigned long long>();
		return json;
	}
	void GlobalyNamedLongMask::set(const Json& json) 
	{
		const auto& it = json.find("mask");
		if(it == json.cend()) 
		{
			return;
		}
		this->valueref<unsigned long long>() = it->get<unsigned long long>();
 	}
	bool GlobalyNamedLongMask::display(const char* label) 
	{
		if(propptr) 
		{
			if(ImGui::CollapsingHeader(label)) 
			{
				bool ret = false;
				for(long long i = 0; i < 64; i++) 
				{
					bool bit = valueref<unsigned long long>() & (1ULL << i);
					std::string label = std::format("###gnlmn{}{}", (long long)this, i);
					ImGui::InputText(label.c_str(), &names[i]);
					ImGui::SameLine();
					if(ImGui::Checkbox((std::format("###gnlmbv{}{}", (long long)this, i)).c_str(), &bit)) 
					{
						valueref<long long>() ^= (1ULL << i);
						ret = true;
					}
				}
				return ret;
			}
		}
		return false;
	}
}