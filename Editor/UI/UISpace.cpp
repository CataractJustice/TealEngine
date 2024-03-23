#include "UISpace.h"
#include "Core.h"
#include "libs/tinyfiledialogs.h"
#include "EditorUI/ComponentsExplorer.h"
#include "EditorUI/GameNodePropsWindow.h"
#include "EditorUI/MaterialEditor.h"
#include "EditorUI/ProjectPropertiesWindow.h"
#include "System/Input.h"


namespace TealEngine 
{
	GameAssetsBrowser* gameAssetsBrowser;
	MaterialEditor* materialEditor;
	Texture playButtonTexture;
	Texture pauseButtonTexture;
	Texture stopButtonTexture;
	UISpace::UISpace() 
	{
	}

	void UISpace::init() 
	{
		playButtonTexture.loadFromFile("./Assets/Images/GameStateIcons/Play.png");
		pauseButtonTexture.loadFromFile("./Assets/Images/GameStateIcons/Pause.png");
		stopButtonTexture.loadFromFile("./Assets/Images/GameStateIcons/Stop.png");
		gameAssetsBrowser = new GameAssetsBrowser();
		materialEditor = new MaterialEditor();
		addWindowOption(EditorWindowNames::gameSceneTree, [](){
			const ImVec2 controllButtonSize = ImVec2(32.0f, 32.0f);
			float availX = ImGui::GetContentRegionAvail().x;
			if(Core::getEngineState() == Core::EngineState::GAME_STOPPED) 
			{
				ImGui::SetCursorPos(ImVec2(availX / 2.0f - controllButtonSize.x / 2.0f, ImGui::GetCursorPosY()));
				if(ImGui::ImageButton(ImTextureID((long)playButtonTexture.id()), controllButtonSize)) 
				{
					Core::play();
				}
			}
			else 
			{
				ImGui::SetCursorPos(ImVec2(availX / 2.0f - controllButtonSize.x, ImGui::GetCursorPosY()));
				if(Core::getEngineState() == Core::EngineState::GAME_PLAYING) 
				{
					if(ImGui::ImageButton(ImTextureID((long)pauseButtonTexture.id()), controllButtonSize)) 
					{
						Core::pause();
					}
				}
				else 
				{
					if(ImGui::ImageButton(ImTextureID((long)playButtonTexture.id()), controllButtonSize)) 
					{
						Core::play();
					}
				}
				ImGui::SameLine();
				ImGui::SetCursorPos(ImVec2(availX / 2.0f, ImGui::GetCursorPosY()));
				if(ImGui::ImageButton(ImTextureID((long)stopButtonTexture.id()), controllButtonSize)) 
				{
					Core::stop();
				}
			}
			Core::getRoot()->displayNodeTree(); 
		});
		addWindowOption(EditorWindowNames::gameNodeProps, GameNodePropsWindow::render);
		addWindowOption(EditorWindowNames::componentsExplorer, ComponentsExplorer::render);
		addWindowOption(EditorWindowNames::gameAssetsBrowser, [](){ gameAssetsBrowser->render(); });
		addWindowOption(EditorWindowNames::projectProps, ProjectPropertiesWindow::render);
		addWindowOption(EditorWindowNames::materialEditor, [](){ materialEditor->render(); });
		addWindowOption(EditorWindowNames::textureViewer, [](){ 
			static int textureId = 0;
			ImGui::InputInt("Texture id", &textureId);
			ImGui::Image(ImTextureID((long)textureId), ImVec2(512.0f, 512.0f));
		});
		addWindowOption(EditorWindowNames::debugInfo, [](){
			if(ImGui::CollapsingHeader("Input")) 
			{
				glm::vec2 mp = Input::Mouse::getMousePos();
				glm::vec2 mpd = Input::Mouse::getDeltaMousePos();
				ImGui::Text("Mouse position: %f | %f", mp.x, mp.y);
				ImGui::Text("Delta mouse position: %f | %f", mpd.x, mpd.y);
				ImGui::Text("Scroll position: %d", (int)Input::Mouse::getScrollPos());
			}
			if(ImGui::CollapsingHeader("Profiling")) 
			{
				Core::profiler.display();
			}
		});

		openWindow(EditorWindowNames::gameSceneTree);
		openWindow(EditorWindowNames::gameViewport);
		openWindow(EditorWindowNames::gameNodeProps);
		openWindow(EditorWindowNames::componentsExplorer);
		openWindow(EditorWindowNames::gameAssetsBrowser);

		projectCreateWindowActive = 0;
		showEditorUI = true;
	}

	void UISpace::display() 
	{
		if(!showEditorUI) return;
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
					ImGui::MenuItem(windowOption.first.c_str(), "", &windowOption.second.first);
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

		for(auto& windowKeyVal : this->windowOptions) 
		{
			if(windowKeyVal.second.first) 
			{
				ImGui::Begin(windowKeyVal.first.c_str(), &windowKeyVal.second.first);
				windowKeyVal.second.second();
				ImGui::End();
			}
		}
	}
	void UISpace::addWindowOption(const std::string& name, std::function<void()> renderFn) 
	{
		auto it = this->windowOptions.find(name);
		if(it == windowOptions.cend()) 
		{
			windowOptions[name].first = false;
			windowOptions[name].second = renderFn;
		}
	}
	void UISpace::openWindow(const std::string& name) 
	{
		auto it = this->windowOptions.find(name);
		if(it != windowOptions.cend()) 
		{
			it->second.first = true;
		}
	}
	void UISpace::closeWindow(const std::string& name) 
	{
		auto it = this->windowOptions.find(name);
		if(it != windowOptions.cend()) 
		{
			it->second.first = false;
		}
	}
}