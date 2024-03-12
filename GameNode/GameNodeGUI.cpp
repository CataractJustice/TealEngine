#include "GameNodeGUI.h"
#include "System/Input.h"
namespace TealEngine 
{
	GameNodeGUI::GameNodeGUI() 
	{
		this->transformProp = new GUITransformProp(&transform);
	}
	Json GameNodeGUI::toJson() 
	{
		Json json = GameNode::toJson();
		json["transform"] = this->transformProp->get();
		json["type"] = "gui";
		return json;
	}
		
	void GameNodeGUI::displayProps() 
	{
		this->transformProp->display("Transform");
		GameNode::displayProps();
	}

	void GameNodeGUI::setParent(GameNode* parent) 
	{
		GameNode::setParent(parent);
		GameNodeGUI* nodeGUI = this->getParentOfType<GameNodeGUI>();
		if(nodeGUI) 
		{
			this->transform.setParent(&nodeGUI->getTransform());
		}
	}

	GUITransform& GameNodeGUI::getTransform() 
	{
		return this->transform;
	}

	void GameNodeGUI::onWindowMousePress(unsigned short button) 
	{
		if(!active) return;
		glm::vec2 mpos = Input::Mouse::getMousePos();
		if(
		mpos.x > getTransform().getLeft().inPixels(transform) &&
		mpos.y > getTransform().getTop().inPixels(transform) &&
		mpos.x < getTransform().getRight().inPixels(transform) &&
		mpos.y < getTransform().getBottom().inPixels(transform)) 
		{
			onMousePress(button);
		}
		GameNode::onWindowMousePress(button);
	}
	void GameNodeGUI::onWindowMouseRelease(unsigned short button) 
	{
		if(!active) return;
		glm::vec2 mpos = Input::Mouse::getMousePos();
		if(
		mpos.x > getTransform().getLeft().inPixels(transform) &&
		mpos.y > getTransform().getTop().inPixels(transform) &&
		mpos.x < getTransform().getRight().inPixels(transform) &&
		mpos.y < getTransform().getBottom().inPixels(transform)) 
		{
			onMouseRelease(button);
		}
		GameNode::onWindowMouseRelease(button);
	}
}
