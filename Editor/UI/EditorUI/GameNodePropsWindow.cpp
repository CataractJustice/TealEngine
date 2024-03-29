#include "GameNodePropsWindow.h"
#include "Editor/UI/UISpace.h"
#include "Core.h"
#include "GameNode/ComponentFactory.h"

namespace TealEngine 
{
	void GameNodePropsWindow::render() 
	{
		if(GameNode::getSelectedNode()) 
		{
			if(ImGui::TreeNode("Debug info")) 
			{
				ImGui::Columns(2);
				ImGui::Text("%s", "global id");
				ImGui::NextColumn();
				ImGui::Text("%d", GameNode::getSelectedNode()->getId());
				ImGui::NextColumn();
				ImGui::Text("%s", "depth");
				ImGui::NextColumn();
				ImGui::Text("%d", GameNode::getSelectedNode()->getHierarchyDepth());
				ImGui::NextColumn();
				ImGui::Columns();
				ImGui::TreePop();
			}
			GameNode::getSelectedNode()->displayProps();
		
			ImVec2 availSpace = ImGui::GetContentRegionAvail();
			if(availSpace.y < 128.0f) availSpace.y = 128.0f;
			ImGui::Dummy(ImVec2(availSpace.x, availSpace.y));
			if(ImGui::BeginDragDropTarget()) 
			{
				if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ComponentClass")) 
				{
					GameNode::getSelectedNode()->attachComponent(ComponentFactory::instanceComponent(std::string((char*)payload->Data, payload->DataSize)));
				}
			}
		}
	}
}