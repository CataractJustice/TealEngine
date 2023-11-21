#include "ProjectPropertiesWindow.h"
#include "Editor/UI/UISpace.h"
#include "Core.h"

namespace TealEngine 
{
	void ProjectPropertiesWindow::render() 
	{
		std::string defaultScene = Core::currentProject.getDefaultScene();
		if(ImGui::InputText("Default scene", &defaultScene)) 
		{
			Core::currentProject.setDefaultScene(defaultScene);
		}
		if(ImGui::Button("Save##projectProps")) 
		{
			Core::currentProject.save();
			Core::uiSpace.closeWindow(EditorWindowNames::projectProps);
		}
		if(ImGui::Button("Cancel##projectProps")) 
		{
			Core::uiSpace.closeWindow(EditorWindowNames::projectProps);
		}
	}
}