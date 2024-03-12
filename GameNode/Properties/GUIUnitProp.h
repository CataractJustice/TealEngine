#pragma once
#include "GameNode/ComponentProp.h"
#include "Graphics/GUI/GUITransform.h"

namespace TealEngine 
{
	class GUIUnitProp : public IProp 
	{
		private:
		std::string str;
		public:
		GUIUnitProp(GUIUnit* unitPtr);
		Json get() override;
		void set(const Json& json) override;
		bool display(const char* label) override;
	};
}
