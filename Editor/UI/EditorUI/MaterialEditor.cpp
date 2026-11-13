#include "MaterialEditor.h"
#include "Editor/UI/UISpace.h"
#include "Core.h"
#include "GameNode/ComponentFactory.h"
#include <fstream>
#include "Graphics/MaterialIO.h"
namespace TealEngine 
{
	MaterialEditor::MaterialEditor() 
	{
		this->currentMaterial = nullptr;
		this->currentMaterialName = "";
		this->currentMaterialFile = "";
	}
	void MaterialEditor::render() 
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
				if(this->currentMaterial->getShaderProgram()->getId() == kval.second.getId()) 
				{
					shaderItem = indexToShaderName.size() - 1;
				}
			}
			if(ImGui::Combo("Shader#materialEditor", &shaderItem, comboShaderList.c_str())) 
			{
				currentMaterial->setProgram(&Core::shadersManager.get(indexToShaderName[shaderItem]));
			}
			if(this->currentMaterial) 
			{
				ImGui::InputText("Material name", &currentMaterialName);
				int uIndex = 0;
				for(Kirino::ShaderProgram::UniformRecord uniform : this->currentMaterial->getShaderProgram()->getUniforms()) 
				{
					switch (uniform.type)
					{
					case GL_FLOAT:
						ImGui::DragFloat(uniform.name.c_str(), (float*)currentMaterial->getUniformDataPtrByIndex(uIndex));
						break;
					case GL_INT:
						ImGui::InputInt(uniform.name.c_str(), (int*)currentMaterial->getUniformDataPtrByIndex(uIndex));
						break;
					case GL_FLOAT_VEC2:
						ImGui::DragFloat2(uniform.name.c_str(), (float*)currentMaterial->getUniformDataPtrByIndex(uIndex));
						break;
					case GL_FLOAT_VEC3:
						ImGui::DragFloat3(uniform.name.c_str(), (float*)currentMaterial->getUniformDataPtrByIndex(uIndex));
						break;
					case GL_FLOAT_VEC4:
						ImGui::DragFloat4(uniform.name.c_str(), (float*)currentMaterial->getUniformDataPtrByIndex(uIndex));
						break;
					default:
						break;
					}
					uIndex++;
				}
				std::vector<Texture*> indexToTexture;
				std::string comboTextureList = "";
				int samplerIndex = 0;
				for(Kirino::ShaderProgram::UniformRecord samplerRecord : this->currentMaterial->getShaderProgram()->getSamplers()) 
				{
					unsigned int sampler = currentMaterial->getSamplerByIndex(samplerIndex);
					int comboTextureItem = -1;
					for(auto& kval : Core::textureManager.getLoadedMap()) 
					{
						comboTextureList += kval.first;
						comboTextureList.append(1, '\0');
						indexToTexture.push_back(&kval.second);
						if(kval.second.id() == sampler) 
						{
							comboTextureItem = indexToTexture.size() - 1;
						}
					}
					if(ImGui::Combo((samplerRecord.name + "##materialEditorTexture").c_str(), &comboTextureItem, comboTextureList.c_str())) 
					{
						unsigned int newSampler = indexToTexture[comboTextureItem]->id();
						currentMaterial->setTexture(samplerRecord.name, &newSampler, 1, 0);
					}
					
					if(comboTextureItem >= 0) 
					{
						ImGui::Image((ImTextureID)((long long)indexToTexture[comboTextureItem]->id()), ImVec2(indexToTexture[comboTextureItem]->getWidth()/indexToTexture[comboTextureItem]->getHeight()*64.0f, 64.0f));
					}
					samplerIndex++;
				}
				if(ImGui::Button("Save##materialEditor")) 
				{
					Kirino::MaterialIO::save(*currentMaterial, currentMaterialName, currentMaterialFile.replace_extension(".mbf"));
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
				Kirino::MaterialIO::MaterialFileInfo info = Kirino::MaterialIO::info((char*)payload->Data);
				
				//if file is not a valid material then make new material
				if(info.valid == false) 
				{
					int newMaterialNumber = 0;
					while(Core::materialsManager.getLoadedMap().find(std::string("NewMaterial") + std::to_string(newMaterialNumber)) != Core::materialsManager.getLoadedMap().cend()) 
					{
						newMaterialNumber++;
					}
					currentMaterialName = "NewMaterial" + std::to_string(newMaterialNumber);
					Kirino::Material& newMat = Core::materialsManager.getLoadedMap()[currentMaterialName];
					newMat.setProgram(&Core::shadersManager.get("Deferred"));
					this->currentMaterial = &Core::materialsManager.get(currentMaterialName);
				}
				else 
				{
					//if material is not loaded - load it
					if(Core::materialsManager.exists(info.name)) Core::materialsManager.load((char*)payload->Data);
					//if file is a valid material then use it
					currentMaterialName = info.name;
					this->currentMaterial = &Core::materialsManager.get(info.name);
				}
				this->currentMaterialFile = std::filesystem::path((char*)payload->Data);
			}
		}
	}
}