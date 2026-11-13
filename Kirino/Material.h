#pragma once
#include "ShaderProgram.h"
#include "UBOBindings.h"
#include "TextureBindings.h"
#include <vector>
namespace Kirino 
{
	class Material 
	{
		private:
		ShaderProgram* program;
		std::vector<void*> savedUniforms;
		std::vector<unsigned int> savedSamplers;
		//Uniform Buffer Objects.
		std::vector<unsigned int> UBOs;

		public:
		Material(ShaderProgram* program);
		Material();
		void setProgram(ShaderProgram* program);
		~Material();
		void setUniform(unsigned int index, const void* value, unsigned int size, const void* offset);
		void setUniform(const std::string& name, const void* value, unsigned int size, const void* offset);
		unsigned int getUniformIndex(const std::string& name);
		void setUniformBlock(const std::string& name, unsigned int ubo);
		void setTexture(const std::string& name, const unsigned int* textures, unsigned int size = 1, unsigned int arrayIndex = 0, bool volume = false);
		void setTextureByIndex(unsigned int index, const unsigned int* textures, unsigned int size = 1, unsigned int arrayIndex = 0, bool volume = false);
		void setSingleTextureByIndex(unsigned int index, unsigned int texture, bool volume = false);
		unsigned int getTextureIndex(const std::string& name) const;
		void getTextures(const std::string& name, unsigned int* textures, unsigned int size = 1, unsigned int arrayIndex = 0) const;
		inline const ShaderProgram* getShaderProgram() const { return program; }
		void useProgram() const;
		void setupUniforms() const;
		void setupUniformBlocks(UBOBindings& bindings) const;
		void setupTextures(TextureBindings& bindings) const;
		inline const void* getUniformDataPtrByIndex(unsigned int index) const { return savedUniforms[index]; };
		inline unsigned int getSamplerByIndex(unsigned int index) const { return savedSamplers[index]; };
		void* getUniformDataPtrByName(const std::string& name);
		unsigned int getSamplerByName(const std::string& name);
	};
}