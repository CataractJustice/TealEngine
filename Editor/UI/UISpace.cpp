#include "UISpace.h"
#include "Core.h"
#include "libs/tinyfiledialogs.h"
namespace TealEngine 
{
	UISpace::UISpace() 
	{
		addWindowOption(EditorWindowNames::editorSceneTree);
		addWindowOption(EditorWindowNames::gameSceneTree);
		addWindowOption(EditorWindowNames::editorViewport);
		addWindowOption(EditorWindowNames::gameViewport);
		addWindowOption(EditorWindowNames::textureDebugger);
		addWindowOption(EditorWindowNames::gameNodeProps);
		addWindowOption(EditorWindowNames::componentsExplorer);
		addWindowOption(EditorWindowNames::gameAssetsBrowser);
		addWindowOption(EditorWindowNames::projectProps);
		addWindowOption(EditorWindowNames::materialEditor);

		openWindow(EditorWindowNames::gameSceneTree);
		openWindow(EditorWindowNames::gameViewport);
		openWindow(EditorWindowNames::gameNodeProps);
		openWindow(EditorWindowNames::componentsExplorer);
		openWindow(EditorWindowNames::gameAssetsBrowser);

		projectCreateWindowActive = 0;
	}

	void UISpace::display() 
	{
		ImGui::DockSpaceOverViewport();
		if(ImGui::BeginMainMenuBar()) 
		{
			if(ImGui::BeginMenu("File")) 
			{
				if(ImGui::MenuItem("Undo", "CTRL + Z")) { Core::actionList.undoAction(); }
				if(ImGui::MenuItem("Redo", "CTRL + Y")) { Core::actionList.redoAction(); }
				if(ImGui::MenuItem("New project")) 
				{
					projectCreateWindowActive = true;
				}
				if(ImGui::MenuItem("Open project")) 
				{
					char* dir = tinyfd_selectFolderDialog("Select project folder", "");
					if(dir)
					{
						Project project = Project::open(dir);
						if(project.error.length() == 0) 
						{
							Core::setProject(project);
						}
					}
				}
				ImGui::EndMenu();
			}

			if(projectCreateWindowActive) 
			{
				ImGui::Begin("Create project", &projectCreateWindowActive);
				ImGui::Text("%s", "Project name");
				ImGui::SameLine();
				ImGui::InputText("##", &projectCreateTitleInput);
				ImGui::SameLine();
				if(ImGui::Button("Create project")) 
				{
					char* dir = tinyfd_selectFolderDialog("Select folder", ".");
					if(dir) 
					{
						Project project = Project::create(dir, projectCreateTitleInput);
						if(project.error.length() == 0) 
						{
							Core::setProject(project);
						}
						projectCreateWindowActive = 0;
					}
				}
				ImGui::End();
			}

			if(ImGui::BeginMenu("Windows")) 
			{
				for(auto& windowOption : windowOptions) 
				{
					ImGui::MenuItem(windowOption.first.c_str(), "", &windowOption.second);
				}
				ImGui::EndMenu();
			}

			if(ImGui::BeginMenu("Project")) 
			{
				if(ImGui::MenuItem("Build modules")) 
				{
					Core::requestModulesReload();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		
		GameNode* root = Core::getRoot();

		for(auto& windowKeyVal : this->windowOptions) 
		{
			if(windowKeyVal.second) 
			{
				ImGui::Begin(windowKeyVal.first.c_str(), &windowKeyVal.second);
				root->imGuiRender(windowKeyVal.first.c_str());
				ImGui::End();
			}
		}
	}
	void UISpace::addWindowOption(const std::string& name) 
	{
		auto it = this->windowOptions.find(name);
		if(it == windowOptions.cend()) 
		{
			windowOptions[name] = false;
		}
	}
	void UISpace::openWindow(const std::string& name) 
	{
		auto it = this->windowOptions.find(name);
		if(it != windowOptions.cend()) 
		{
			it->second = true;
		}
	}
	void UISpace::closeWindow(const std::string& name) 
	{
		auto it = this->windowOptions.find(name);
		if(it != windowOptions.cend()) 
		{
			it->second = false;
		}
	}
}