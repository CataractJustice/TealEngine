#pragma once
#include "GameNode/Component.h"
namespace TealEngine 
{
	class SpaceBetween : public Component 
	{
		private:
		void updateChildrenTransform();
		public:
		void GUIRender() override;
	};
}