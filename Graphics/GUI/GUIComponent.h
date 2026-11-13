#pragma once
#include "GameNode/Component.h"
#include "GameNode/GameNodeGUI.h"
namespace TealEngine 
{
	class GUIComponent : public Component
	{
		private:
		GameNodeGUI* gui;
		public:
		GUIComponent();
		virtual void redraw();
		void onAttach() override;
		void onAwake() override;
		void onSleep() override;
		void onDestroy() override;
		void onPropSet(const std::string& name) override;
	};
}