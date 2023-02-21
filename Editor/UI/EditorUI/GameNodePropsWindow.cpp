#include "GameNodePropsWindow.h"
#include "Editor/UI/UISpace.h"
#include "Core.h"
#include "GameNode/ComponentFactory.h"

namespace TealEngine 
{
	void GameNodePropsWindow::imGuiRender(const std::string& windowName) 
	{
		if(windowName == EditorWindowNames::gameNodeProps) 
		{
			if(GameNode::getSelectedNode()) 
			{
				GameNode::getSelectedNode()->displayProps();
			}
			
			ImVec2 availSpace = ImGui::GetContentRegionAvail();
			if(availSpace.y < 128.0f) availSpace.y = 128.0f;
			ImGui::Dummy(ImVec2(availSpace.x, availSpace.y));
			if(ImGui::BeginDragDropTarget()) 
			{
				if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ComponentClass")) 
				{
					GameNode::getSelectedNode()->attachComponent(ComponentFactory::instanceComponent((char*)payload->Data));
				}
			}
		}
	}
}