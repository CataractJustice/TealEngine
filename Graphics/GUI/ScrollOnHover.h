#pragma once
#include "GUIComponent.h"
namespace TealEngine
{
	class ScrollOnHover : public GUIComponent 
	{
		private:
		public:
		GUIUnit scrollStep;
		ScrollOnHover();

		void update() override;
	};
} // namespace TealEngine
