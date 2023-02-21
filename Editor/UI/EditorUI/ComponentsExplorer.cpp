#include "ComponentsExplorer.h"
#include "Editor/UI/UISpace.h"
#include "GameNode/ComponentFactory.h"
#include "Core.h"

namespace TealEngine 
{
	void ComponentsExplorer::imGuiRender(const std::string& windowName) 
	{
		if(windowName == EditorWindowNames::componentsExplorer) 
		{
			int componentsCount = ComponentFactory::componentClassesCount();
			const char** componentClasses = ComponentFactory::cStrComponentClassesArray();

			for(int i = 0; i < componentsCount; i++) 
			{
				const char* componentClass = componentClasses[i];
				ImGui::Button(componentClass);
				if(ImGui::BeginDragDropSource())
				{
					ImGui::TextUnformatted(componentClass);
					ImGui::SetDragDropPayload("ComponentClass", componentClass, std::strlen(componentClass));
					ImGui::EndDragDropSource();
				}
			}
		}
	}
}