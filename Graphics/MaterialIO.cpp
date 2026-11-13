#include "MaterialIO.h"
#include "FileSystem/loadBytesFromFile.h"
#include "Core.h"
#include <fstream>
namespace Kirino 
{
	void MaterialLoaderJson::load(std::filesystem::path path) 
	{

	}

	MaterialLoaderV1::LoadStatus MaterialLoaderV1::load(Kirino::Material& material, std::string& name, std::filesystem::path path)
	{
		MaterialLoaderV1::LoadStatus status;
		status.ok = false;
		const unsigned int magic = MaterialFormatMagic::v1x;
		const unsigned int version = 1;
		std::vector<char> bytes = loadBytesFromFile(path);
		if(bytes.size() < sizeof(MaterialFormatHeaderV1)) 
		{
			//"Invalid format, file is too short"
			status.loadError = LoadError::FileTooShort;
			return status;
		}
		MaterialFormatHeaderV1 header = *((MaterialFormatHeaderV1*)bytes.data());
		if(header.magic != magic) 
		{
			//"Invalid format"
			status.loadError = LoadError::InvalidFormatStart;
			return status;
		}
		if(header.version != version) 
		{
			//"Invalid version"
			status.loadError = LoadError::InvalidVersion;
			return status;
		}

		if(header.samplerCount > 256) 
		{
			//"Invalid sampler count"
			status.loadError = LoadError::InvalidSamplerCount;
			return status;
		}

		if(header.uniformsCount > 256) 
		{
			//"Invalid uniforms count"
			status.loadError = LoadError::InvalidUniformCount;
			return status;
		} 

		if(TealEngine::Core::shadersManager.exists(header.shader)) 
		{
			material.setProgram(&TealEngine::Core::shadersManager.get(header.shader));
		}
		else 
		{
			//"Unknow shader"
			status.loadError = LoadError::UnknownShader;
			return status;
		}

		unsigned int cursor = sizeof(MaterialFormatHeaderV1);

		for(int i = 0; i < header.samplerCount; i++) 
		{
			unsigned int samplerNameSize = *((unsigned int*)(bytes.data() + cursor));
			cursor += sizeof(samplerNameSize);
			
			std::string samplerName = std::string((bytes.data() + cursor), samplerNameSize);
			cursor += samplerNameSize;

			unsigned int samplerTextureNameSize = *((unsigned int*)(bytes.data() + cursor));
			cursor += sizeof(samplerTextureNameSize);
			
			std::string samplerTextureName = std::string(bytes.data() + cursor, samplerTextureNameSize);
			cursor += samplerTextureNameSize;
			
			if(TealEngine::Core::textureManager.exists(samplerTextureName)) 
			{
				unsigned int texture = TealEngine::Core::textureManager.get(samplerTextureName).id();
				material.setTexture(samplerName, &texture, 1);
			}
		}
		for(int i = 0; i < header.uniformsCount; i++) 
		{
			unsigned int uniformNameSize = *((unsigned int*)(bytes.data() + cursor));
			cursor += sizeof(uniformNameSize);
			
			std::string uniformName = std::string((bytes.data() + cursor), uniformNameSize);
			cursor += uniformNameSize;

			unsigned int uniformDataSize = *((unsigned int*)(bytes.data() + cursor));
			cursor += sizeof(uniformDataSize);
			
			char* uniformDataPtr = bytes.data() + cursor;
			cursor += uniformDataSize;
			const ShaderProgram::UniformRecord* ur = material.getShaderProgram()->getUniformRecordByName(uniformName);
			if(ur == nullptr) continue;
			if(ur->byteSize * ur->arraySize != uniformDataSize) continue;
			
			material.setUniform(uniformName, uniformDataPtr, uniformDataSize, nullptr);
		}
		name = header.name;
		status.loadError = LoadError::Ok;
		status.ok = true;
		return status;
	}
	void MaterialLoaderV1::save(const Kirino::Material& material, const std::string& name, std::filesystem::path path) 
	{
		MaterialFormatHeaderV1 header;
		header.magic = MaterialFormatMagic::v1x;
		header.version = 1;
		header.samplerCount = material.getShaderProgram()->getSamplers().size();
		header.uniformsCount = material.getShaderProgram()->getUniforms().size();
		memset(header.name, 0, sizeof(header.name));
		memcpy(header.name, name.c_str(), name.size());
		memset(header.shader, 0, sizeof(header.shader));
		auto& shaders = TealEngine::Core::shadersManager.getLoadedMap();
		for(auto&[shaderName, shader] : shaders) 
		{
			if(shader.getId() == material.getShaderProgram()->getId()) 
			{
				memcpy(header.shader, shaderName.c_str(), shaderName.size());
				break;
			}
		}
		std::vector<char> bytes;
		bytes.insert(bytes.end(), (char*)&header, ((char*)&header) + sizeof(header));
		//insert sampler data
		for(const ShaderProgram::UniformRecord& sampler : material.getShaderProgram()->getSamplers()) 
		{
			unsigned int textureId;
			material.getTextures(sampler.name, &textureId, 1, 0);
			unsigned int samplerNameSize = sampler.name.size();
			std::string samplerTextureName = "";
			auto& textureMap = TealEngine::Core::textureManager.getLoadedMap();
			for(auto&[textureName, texture] : textureMap) 
			{
				if(texture.id() == textureId) 
				{
					samplerTextureName = textureName;
					break;
				}
			}
			unsigned int samplerTextureNameSize = samplerTextureName.size();
			bytes.insert(bytes.end(), (char*)&samplerNameSize, ((char*)&samplerNameSize) + sizeof(samplerNameSize));
			bytes.insert(bytes.end(), sampler.name.c_str(), sampler.name.c_str() + sampler.name.size());
			bytes.insert(bytes.end(), (char*)&samplerTextureNameSize, ((char*)&samplerTextureNameSize) + sizeof(samplerTextureNameSize));
			bytes.insert(bytes.end(), samplerTextureName.c_str(), samplerTextureName.c_str() + samplerTextureName.size());
		}
		//insert uniform data
		int uniformIndex = 0;
		for(const ShaderProgram::UniformRecord& uniform : material.getShaderProgram()->getUniforms()) 
		{
			unsigned int uniformSize = uniform.arraySize * uniform.byteSize;
			unsigned int uniformNameSize = uniform.name.size();
			bytes.insert(bytes.end(), (char*)&uniformNameSize, ((char*)&uniformNameSize) + sizeof(uniformNameSize));
			bytes.insert(bytes.end(), uniform.name.c_str(), uniform.name.c_str() + uniform.name.size());
			bytes.insert(bytes.end(), (char*)&uniformSize, ((char*)&uniformSize) + sizeof(uniformSize));
			bytes.insert(bytes.end(), (char*)material.getUniformDataPtrByIndex(uniformIndex), (char*)material.getUniformDataPtrByIndex(uniformIndex) + uniformSize);
			uniformIndex++;
		}

		std::ofstream materialFile(path, std::ios::binary);
		materialFile.write(bytes.data(), bytes.size());
	}
	MaterialIO::MaterialFileInfo MaterialLoaderV1::info(const std::filesystem::path& path) 
	{
		MaterialIO::MaterialFileInfo info;
		info.valid = false;
		info.name = "";
		MaterialFormatHeaderV1 header;
		std::vector<char> bytes = loadBytesFromFile(path);
		if(bytes.size() < sizeof(header)) 
		{
			return info;
		}
		header = *(MaterialFormatHeaderV1*)bytes.data();
		if(header.magic != MaterialFormatMagic::v1x || header.version != 1) 
		{
			return info;
		}
		info.name = header.name;
		info.valid = true;
		return info;
	}

	void MaterialIO::load(Kirino::Material& material, std::string& name, const std::filesystem::path& path) 
	{
		MaterialLoaderV1::load(material, name, path);
	}
	void MaterialIO::save(const Kirino::Material& material, const std::string& name, const std::filesystem::path& path) 
	{
		MaterialLoaderV1::save(material, name, path);
	}
	MaterialIO::MaterialFileInfo MaterialIO::info(const std::filesystem::path& path) 
	{
		return MaterialLoaderV1::info(path);
	}
}