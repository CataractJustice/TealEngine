#include "CameraViewport.h"
#include "Editor/UI/UISpace.h"
#include "Core.h"
#include "GameNode/ComponentFactory.h"
namespace TealEngine 
{
	CameraViewport::CameraViewport(const std::string& windowName) :
	windowName(windowName)
	{
		testTextureId = 0;
	}
	void CameraViewport::imGuiRender(const std::string& windowName) 
	{
		if(windowName == this->windowName) 
		{
			if(ImGui::Button("Run")) Core::play();
			if(ImGui::Button("Pause")) Core::pause();
			if(ImGui::Button("Stop")) Core::stop();
			ImGui::InputInt("view texture", &testTextureId);

			if(Core::renderer.getActiveCamera()) 
			{
				Texture& frame = Core::renderer.getActiveCamera()->renderTexture;
				ImVec2 frameSize = ImVec2(frame.getWidth(), frame.getHeight());
				ImVec2 cursorPos = ImGui::GetCursorPos();
				ImVec2 area = ImGui::GetContentRegionAvail();

				glm::vec2 imagePos = glm::vec2(cursorPos.x, cursorPos.y) + glm::vec2(area.x - frameSize.x, area.y - frameSize.y) * 0.5f;
				ImGui::SetCursorPos(ImVec2(imagePos.x, imagePos.y));
				ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
				ImGui::Image(ImTextureID((long)(testTextureId)), frameSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
				if(ImGui::BeginDragDropTarget()) 
				{
					if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ComponentClass")) 
					{
						glm::ivec2 pixel = glm::ivec2(Input::Mouse::getMousePos().x - cursorScreenPos.x, Input::Mouse::getMousePos().y - cursorScreenPos.y);
						int id = Core::idRenderer.getActiveCamera()->renderTexture.getInt32(pixel.x, pixel.y);
						if(id) GameNode::getNodeById(id)->attachComponent(ComponentFactory::instanceComponent((char*)payload->Data));
					}
				}
				if(ImGui::IsItemClicked(0)) 
				{
					glm::ivec2 pixel = glm::ivec2(Input::Mouse::getMousePos().x - cursorScreenPos.x, Input::Mouse::getMousePos().y - cursorScreenPos.y);
					int id = Core::idRenderer.getActiveCamera()->renderTexture.getInt32(pixel.x, pixel.y);
					if(id) GameNode::setSelectedNode(GameNode::getNodeById(id));
				}
			}
			else 
			{
				ImGui::TextUnformatted("No camera");
			}
		}
	}
}