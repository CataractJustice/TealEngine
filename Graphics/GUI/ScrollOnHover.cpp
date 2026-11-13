#include "ScrollOnHover.h"
#include "System/Input.h"
namespace TealEngine 
{
	ScrollOnHover::ScrollOnHover() 
	{
		this->addProp(new GUIUnitProp(&scrollStep), "Scroll step");
	}
	void ScrollOnHover::update() 
	{
		double scroll = Input::Mouse::getDeltaScrollPos();
		if(scroll == 0.0) return;
		GameNodeGUI* gui = this->getParentOfType<GameNodeGUI>();
		if(!gui) return;
		const glm::vec2 mpos = Input::Mouse::getMousePos();
		if(!gui->isOverElement(mpos.x, mpos.y, false)) return;
		gui->getTransform().setScrollY(gui->getTransform().getScrollY() + (scroll > 0.0 ? -scrollStep : scrollStep));
		redraw();
	}
}