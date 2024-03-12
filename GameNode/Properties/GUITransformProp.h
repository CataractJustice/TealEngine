#pragma once
#include "GameNode/ComponentProp.h"
#include "Graphics/GUI/GUITransform.h"
#include "GUIUnitProp.h"

namespace TealEngine 
{
	class GUITransformProp : public IProp 
	{
		private:
		GUIUnitProp x,y,w,h;
		public:
		GUITransformProp(GUITransform* transformPtr);
		Json get() override;
		void set(const Json& json) override;
		bool display(const char* label) override;
	};
}
