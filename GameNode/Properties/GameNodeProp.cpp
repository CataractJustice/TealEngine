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
		json["path"] = this->ownerComp->getParent()->getRelativeIndexPathTo(this->valueref<GameNode*>());
		return json;
	}
	void GameNodeProp::set(const Json& json) 
	{
		const auto& it = json.find("path");
		if(it == json.cend()) 
		{
			this->valueref<GameNode*>() = nullptr;
			return;
		}
		std::vector<int>path = it->get<std::vector<int>>();
		GameNode* nodeIt = this->ownerComp->getParent();
		for(auto i : path) 
		{
			if(i == -1) 
			{
				nodeIt = nodeIt->getParent();
			}
			else 
			{
				nodeIt = nodeIt->getChildByIndex(i);
			}
			if(nodeIt == nullptr) break;
		}
		this->valueref<GameNode*>() = nodeIt;
 	}
	bool GameNodeProp::display(const char* label) 
	{
		GameNode* node = nullptr;
		if(propptr) 
		{
			if(GameNode::isValidNode(valueref<GameNode*>())) 
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