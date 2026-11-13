#include "GUIComponent.h"

namespace TealEngine 
{
	GUIComponent::GUIComponent() : gui(nullptr) {};
	void GUIComponent::redraw() 
	{
		if(gui) gui->triggerRedrawComponents();
	}
	void GUIComponent::onAttach()	
	{
		if(this->getParent())
			gui = dynamic_cast<GameNodeGUI*>(this->getParent());
		redraw();
	}
	void GUIComponent::onAwake()	
	{
		redraw();
	}
	void GUIComponent::onSleep()	
	{
		redraw();
	}
	void GUIComponent::onDestroy()	
	{
		redraw();
	}
	void GUIComponent::onPropSet(const std::string& name) 
	{
		redraw();
	}
}