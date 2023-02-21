#include "NodeCreateAction.h"
#include "GameNode/GameNode3D.h"
namespace TealEngine 
{
	void NodeCreateAction::undo() 
	{
		this->nodeJson = this->node->toJson();
		this->parentId = this->node->getParent()->getId();
		this->node->destroy();
	}

	void NodeCreateAction::redo() 
	{
		this->node = GameNode3D::nodeFromJson(this->nodeJson);
		GameNode::getNodeById(this->parentId)->addChild(this->node);
	}

	NodeCreateAction::NodeCreateAction(GameNode* node) 
	{
		this->node = node;
		this->nodeJson = this->node->toJson();
		this->parentId = this->node->getParent()->getId();
	}
}