#include "GameNodeProp.h"
#include "GameNode/Component.h"
namespace TealEngine 
{
	GameNodeProp::GameNodeProp(GameNode** nodePtr)
	{
		this->propptr = nodePtr;
	}
	
	Json GameNodeProp::get() 
	{
		Json json;
		if(this->valueref<GameNode*>() == nullptr) 
		{
			return json;
		}
		unsigned int groupId = valueref<GameNode*>()->getGroupId();
		if(groupId == this->ownerComp->getParent()->getGroupId()) 
		{
			json["local"] = 1;
			json["id"] = valueref<GameNode*>()->getGroupLocalId();
		}
		else 
		{
			json["local"] = 0;
			json["id"] = valueref<GameNode*>()->getId();
		}
		return json;
	}
	void GameNodeProp::set(const Json& json) 
	{
		int isLocal = json["local"];
		if(isLocal) 
		{
			this->valueref<GameNode*>() = GameNode::getNodeByGroupAndLocalId(this->ownerComp->getParent()->getGroupId(), json["id"]);
		}
		else 
		{
			this->valueref<GameNode*>() = GameNode::getNodeById(json["id"]);
		}
	}
	bool GameNodeProp::display(const char* label) 
	{
		GameNode* node = nullptr;
		if(propptr) 
		{
			if(GameNode::isNodeExist(valueref<GameNode*>())) 
			{
				node = valueref<GameNode*>();
			}
		}
		
		if(node) 
		{
			ImGui::Text("%s", node->getName().c_str());	
			if(ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemClicked(0)) 
			{
				GameNode::setSelectedNode(node);
			}
		} 
		else 
		{
			ImGui::Text("%s", "<Drop node here>");
		}
		if(ImGui::BeginDragDropTarget()) 
		{
			if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameNode")) 
			{
				this->valueref<GameNode*>() = *((GameNode**)payload->Data);
				return true;
			}
			ImGui::EndDragDropTarget();
		}

		return false;
	}
}