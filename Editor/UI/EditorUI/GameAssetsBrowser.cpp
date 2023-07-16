#include "GameAssetsBrowser.h"
#include "../UISpace.h"
#include <fstream>
#include "Core.h"
#include "GameNode/GameNode3D.h"

namespace TealEngine 
{
	GameAssetsBrowser::GameAssetsBrowser() 
	{
		if(!GameAssetsBrowser::isResourcesLoaded) 
		{
			GameAssetsBrowser::folderIcon.loadFromFile("./Assets/Images/BrowserIcons/Folder.png");
			GameAssetsBrowser::textIcon.loadFromFile("./Assets/Images/BrowserIcons/Text.png");
			GameAssetsBrowser::cppIcon.loadFromFile("./Assets/Images/BrowserIcons/Cpp.png");
			GameAssetsBrowser::imageIcon.loadFromFile("./Assets/Images/BrowserIcons/Image.png");
			GameAssetsBrowser::nodesIcon.loadFromFile("./Assets/Images/BrowserIcons/Nodes.png");
			GameAssetsBrowser::unknownIcon.loadFromFile("./Assets/Images/BrowserIcons/Unknown.png");
			GameAssetsBrowser::isResourcesLoaded = true;
		}
		this->renameTarget = "";
		this->renameInput = "";
	}

	void GameAssetsBrowser::setPath(std::filesystem::path path) 
	{
		this->currentPath = path;
	}

	void GameAssetsBrowser::setRootPath(std::filesystem::path path) 
	{
		this->rootPath = path;
	}

	void GameAssetsBrowser::imGuiRender(const std::string& windowName) 
	{
		static float entrySize = 98.0f;
		static float padding = 8.0f;
		float columnWidth = entrySize + padding;
		
		if(windowName == EditorWindowNames::gameAssetsBrowser) 
		{
			if(ImGui::BeginPopupContextWindow()) 
			{
				if(ImGui::MenuItem("New folder")) 
				{
					std::filesystem::create_directory(this->currentPath/".new_directory_temp_name__");
					this->renameTarget = ".new_directory_temp_name__";
					this->renameInput = "New folder";
					isFocusedRenameInputOnce = false;
				}
				if(ImGui::MenuItem("New file")) 
				{
					std::ofstream newFile(this->currentPath/".new_file_temp_name__");
					this->renameTarget = ".new_file_temp_name__";
					this->renameInput = "New file";
					isFocusedRenameInputOnce = false;
				}
				ImGui::EndPopup();
			}

			if(this->rootPath != this->currentPath) 
			{
				if(ImGui::Button("<")) 
				{
					this->setPath(this->currentPath.parent_path());
				}
			}
			float availWidth = ImGui::GetContentRegionAvail().x;
			int columns = availWidth / columnWidth;
			if(columns < 1) columns = 1;
			ImGui::Columns(columns, 0, false);
			bool renameFinished = false;
			for(auto file : std::filesystem::directory_iterator(this->currentPath)) 
			{
				std::string filename = file.path().filename();
				std::string extension = file.path().extension();

				unsigned int iconId = GameAssetsBrowser::unknownIcon.id();
				
				if(file.is_directory()) iconId = GameAssetsBrowser::folderIcon.id();
				else 
				{
					if(extension == ".txt") 
					{
						iconId = GameAssetsBrowser::textIcon.id();
					}
					else
					if(extension == ".cpp") 
					{
						iconId = GameAssetsBrowser::cppIcon.id();
					}
					else
					if(extension == ".png", extension == ".jpg", extension == ".jpeg", extension == ".bmp") 
					{
						iconId = GameAssetsBrowser::imageIcon.id();
					}
					else
					if(extension == ".node") 
					{
						iconId = GameAssetsBrowser::nodesIcon.id();
					}
				}
				ImGui::PushID(filename.c_str());
				if(ImGui::ImageButton(ImTextureID((long)iconId), ImVec2(entrySize, entrySize), ImVec2(0, 1), ImVec2(1, 0))) 
				{
					if(std::filesystem::is_directory(file)) 
					{
						this->setPath(file.path());
					}
				}
				ImGui::PopID();
				if(extension == ".node") 
				{
					if(ImGui::BeginDragDropSource()) 
					{
						ImGui::SetDragDropPayload("NodeFilePath", file.path().c_str(), file.path().string().length() + 1);
						ImGui::EndDragDropSource();
					}
				}

				if(extension == ".mtljson") 
				{
					if(ImGui::BeginDragDropSource()) 
					{
						ImGui::SetDragDropPayload("MaterialPath", file.path().c_str(), file.path().string().length() + 1);
						ImGui::EndDragDropSource();
					}
				}
				if(ImGui::BeginPopupContextItem()) 
				{
					if(ImGui::MenuItem("Rename")) 
					{
						this->renameInput = this->renameTarget = filename;
						isFocusedRenameInputOnce = false;
					}
					if(ImGui::MenuItem("Delete")) 
					{
						std::filesystem::remove(file.path());
					}
					if(extension == ".node") 
					{
						if(ImGui::MenuItem("Load as scene root")) 
						{
							Core::setScene(GameNode3D::loadNodeFromJsonFile(file.path()));
						}
					}
					ImGui::EndPopup();
				}
				if(this->renameTarget == filename) 
				{
					if(!isFocusedRenameInputOnce) 
					{
						ImGui::SetKeyboardFocusHere(0);
					}
					ImGui::InputText("##renameInput", &renameInput);
					if((!ImGui::IsItemFocused() || !ImGui::IsItemActive()) && isFocusedRenameInputOnce) 
					{
						renameFinished = true;
					}
					
					isFocusedRenameInputOnce = true;	
				}
				else
				{
					ImGui::Text("%s", filename.c_str());
				}
				ImGui::NextColumn();
			}
			if(renameFinished) 
			{
				if(renameInput.length())
					std::filesystem::rename(this->currentPath/this->renameTarget, this->currentPath/this->renameInput);
				this->renameTarget = "";
			}
			ImGui::Columns();
			ImVec2 dropSpace = ImGui::GetContentRegionAvail();
			if(dropSpace.y < entrySize) dropSpace.y = entrySize;
			ImGui::Dummy(dropSpace);
			if(ImGui::BeginDragDropTarget()) 
			{
				if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameNode")) 
				{
					GameNode* node = *((GameNode**)payload->Data);
					node->saveAs(this->currentPath/(node->getName()+".node"));
				}
				ImGui::EndDragDropTarget();
			}
		}
	}

	Texture 
	GameAssetsBrowser::folderIcon, 
	GameAssetsBrowser::textIcon, 
	GameAssetsBrowser::cppIcon, 
	GameAssetsBrowser::imageIcon, 
	GameAssetsBrowser::nodesIcon, 
	GameAssetsBrowser::unknownIcon;

	bool GameAssetsBrowser::isResourcesLoaded = false;
}