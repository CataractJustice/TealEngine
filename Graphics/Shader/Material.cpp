#include "Material.h"
#include "Resources.h"
#include "GameNode/ComponentFactory.h"
#include "GameNode/Properties/TextureSelectProp.h"

namespace TealEngine 
{
	Material::Material() 
	{
		addProp(new StringKeyMapValuePointerProp(&shader, &Resources::getShadersMap()), "Shader");
		addProp(new StringProp(&materialName), "Material name");
		materialName = "";
		oldName = "";
		shader = oldShader = material = nullptr;
	}

	std::string autoMaterialName() 
	{
		std::string materialName;
		int i = 1;
		do 
		{
			materialName = "Material" + std::to_string(i++);
		} 
		while(Resources::isResourceExist(Resources::MATERIAL_RESOURCE, materialName));
		return materialName;
	}

	void Material::onPropSet(const std::string& propName) 
	{
		if(propName == "Shader" && oldShader != shader) 
		{
			//remove every prop except the "Shader" prop
			IProp* shaderProp = props["Shader"];
			this->props.erase("Shader");
			for(auto& prop : props) 
			{
				delete prop.second;
			}
			props.clear();
			addProp(shaderProp, "Shader");
			addProp(new StringProp(&materialName), "Material name");
			
			if(Resources::isResourceExist(Resources::MATERIAL_RESOURCE, materialName) && &Resources::getMaterial(materialName) != material) 
			{
				materialName = autoMaterialName();
			}
			if(materialName.length() == 0) 
			{
				materialName = autoMaterialName();
			}

			Resources::addMaterial(this->materialName, ShaderProgram(*(this->shader)));
			this->material = &Resources::getMaterial(materialName);
			//add shader uniforms as component props
			//"u" stands for Uniform, "t" stands for texture
			auto& umap = material->getUniformsMap();
			auto& tmap = material->getTextureMap();

			for(auto& ukeyval : umap) 
			{
				switch(ukeyval.second.getType()) 
				{
				case GL_FLOAT:
					addProp(new FloatProp((float*)ukeyval.second.valueptr()), ukeyval.first);
					break;
				case GL_INT:
					addProp(new IntProp((int*)ukeyval.second.valueptr()), ukeyval.first);
					break;
				case GL_FLOAT_VEC2:
					addProp(new FloatVecProp((float*)ukeyval.second.valueptr(), 2), ukeyval.first);
					break;
				case GL_FLOAT_VEC3:
					addProp(new FloatVecProp((float*)ukeyval.second.valueptr(), 3), ukeyval.first);
					break;
				case GL_FLOAT_VEC4:
					addProp(new FloatVecProp((float*)ukeyval.second.valueptr(), 4), ukeyval.first);
					break;
				case GL_FLOAT_MAT4:
					break;
				default:
					break;
				}
			}
			textures.clear();
			for(auto& tkeyval : tmap) 
			{
				textures[tkeyval.first] = nullptr;
				addProp(new TextureSelectProp(&textures[tkeyval.first], &Resources::getTexturesMap()), tkeyval.first.c_str());
			}

			oldShader = shader;
		}

		if(propName == "Material name" && this->material) 
		{
			if(Resources::isResourceExist(Resources::MATERIAL_RESOURCE, materialName))  
			{
				if(&Resources::getMaterial(materialName) == this->material) goto materialIsStillSame;
			}
			//check if current material exists in resources and if there is no existing material with new name given to this material
			if(Resources::isResourceExist(Resources::MATERIAL_RESOURCE, oldName) && !Resources::isResourceExist(Resources::MATERIAL_RESOURCE, materialName)) 
			{
				//check if old name was the name of this material in the resources
				if(&Resources::getMaterial(oldName) == this->material)
				{
					Resources::renameResouce(Resources::MATERIAL_RESOURCE, oldName, materialName);
					oldName = materialName;
				}
			}
			else if(!Resources::isResourceExist(Resources::MATERIAL_RESOURCE, oldName))
			{
				//
				Resources::addMaterial(materialName, *(this->material));
				oldName = materialName;
			}
		}
		materialIsStillSame:

		for(auto& tkeyval : textures) 
		{
			if(tkeyval.second) 
			{
				this->material->setTexture(tkeyval.first, tkeyval.second->id());
			}
		}
	}

	EXPORT_COMPONENT(Material);
}