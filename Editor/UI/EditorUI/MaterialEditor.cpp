#include "MaterialEditor.h"
#include "Editor/UI/UISpace.h"
#include "Core.h"
#include "GameNode/ComponentFactory.h"
#include <fstream>

namespace TealEngine 
{
	MaterialEditor::MaterialEditor() 
	{
		this->currentMaterial = nullptr;
		this->currentMaterialName = "";
		this->currentMaterialFile = "";
	}
	void MaterialEditor::imGuiRender(const std::string& windowName) 
	{
		if(windowName == EditorWindowNames::materialEditor) 
		{
			if(std::filesystem::is_regular_file(this->currentMaterialFile)) 
			{
				std::string comboShaderList = "";
				std::vector<std::string> indexToShaderName;
				shaderItem = -1;
				for(const auto& kval : Core::shadersManager.getLoadedMap()) 
				{
					comboShaderList += kval.first;
					comboShaderList.append(1, '\0');
					indexToShaderName.push_back(kval.first);
					if(this->currentMaterial->id() == kval.second.id()) 
					{
						shaderItem = indexToShaderName.size() - 1;
					}
				}
				if(ImGui::Combo("Shader#materialEditor", &shaderItem, comboShaderList.c_str())) 
				{
					this->currentMaterial = &(Core::materialsManager.getLoadedMap()[currentMaterialName] = Core::shadersManager.get(indexToShaderName[shaderItem]));
				}
				if(this->currentMaterial) 
				{
					ImGui::InputText("Material name", &currentMaterialName);

					for(auto& uniform : this->currentMaterial->getUniformsMap()) 
					{
						switch (uniform.second.getType())
						{
						case GL_FLOAT:
							ImGui::DragFloat(uniform.first.c_str(), (float*)uniform.second.valueptr());
							break;
						case GL_INT:
							ImGui::InputInt(uniform.first.c_str(), (int*)uniform.second.valueptr());
							break;
						case GL_FLOAT_VEC2:
							ImGui::DragFloat2(uniform.first.c_str(), (float*)uniform.second.valueptr());
							break;
						case GL_FLOAT_VEC3:
							ImGui::DragFloat3(uniform.first.c_str(), (float*)uniform.second.valueptr());
							break;
						case GL_FLOAT_VEC4:
							ImGui::DragFloat4(uniform.first.c_str(), (float*)uniform.second.valueptr());
							break;
						default:
							break;
						}
					}

					std::vector<Texture*> indexToTexture;
					std::string comboTextureList = "";
					for(auto& texture : this->currentMaterial->getTextureMap()) 
					{
						int comboTextureItem = -1;
						for(auto& kval : Core::textureManager.getLoadedMap()) 
						{
							comboTextureList += kval.first;
							comboTextureList.append(1, '\0');
							indexToTexture.push_back(&kval.second);
							if(kval.second.id() == texture.second.second) 
							{
								comboTextureItem = indexToTexture.size() - 1;
							}
						}
						if(ImGui::Combo((texture.first + "##materialEditorTexture").c_str(), &comboTextureItem, comboTextureList.c_str())) 
						{
							texture.second.second = indexToTexture[comboTextureItem]->id();
						}
						
						
						
						if(comboTextureItem >= 0) 
						{
							ImGui::Image((ImTextureID)((long)indexToTexture[comboTextureItem]->id()), ImVec2(indexToTexture[comboTextureItem]->getWidth()/indexToTexture[comboTextureItem]->getHeight()*32.0f, 32.0f));
						}
					}

					if(ImGui::Button("Save##materialEditor")) 
					{
						this->currentMaterial->saveMaterial(this->currentMaterialFile, this->currentMaterialName);
					}
				}
			}

			ImVec2 availSpace = ImGui::GetContentRegionAvail();
			if(availSpace.y < 128.0f) availSpace.y = 128.0f;
			ImGui::Dummy(ImVec2(availSpace.x, availSpace.y));
			if(ImGui::BeginDragDropTarget()) 
			{
				if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MaterialPath")) 
				{
					std::ifstream jsonFile((char*)payload->Data);
					Json json;
					bool validJson = true;
					try 
					{
						json = Json::parse(jsonFile);
					}
					catch (const nlohmann::json::parse_error& e)
					{
						validJson = false;
					}
					Core::materialsManager.load((char*)payload->Data);
					if(!validJson || (json.find("name") == json.cend()) || Core::materialsManager.getLoadedMap().find(json["name"].get<std::string>()) == Core::materialsManager.getLoadedMap().cend()) 
					{
						//if file is not a valid material then make new material
						int newMaterialNumber = 0;
						while(Core::materialsManager.getLoadedMap().find(std::string("NewMaterial") + std::to_string(newMaterialNumber)) != Core::materialsManager.getLoadedMap().cend()) 
						{
							newMaterialNumber++;
						}
						currentMaterialName = "NewMaterial" + std::to_string(newMaterialNumber);
						Core::materialsManager.getLoadedMap()[currentMaterialName] = Core::shadersManager.get("Deferred");
						this->currentMaterial = &Core::materialsManager.get(currentMaterialName);
					}
					else 
					{
						//if file is a valid material then use it
						currentMaterialName = json["name"];
						this->currentMaterial = &Core::materialsManager.get(json["name"]);
					}
					this->currentMaterialFile = std::filesystem::path((char*)payload->Data);
				}
			}
		}
	}
}