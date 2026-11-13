#include <GL/glew.h>
#include "Material.h"
#include <memory.h>

void glUniformAuto(int location, int arraySize, const void* value, GLenum type);

namespace Kirino
{
	void Material::setUniform(unsigned int index, const void* value, unsigned int size, const void* offset) 
	{
		const auto& u = program->getUniforms()[index];
		unsigned int offsetU = *(unsigned int*)&offset;
		memcpy((char*)savedUniforms[index] + (long long)offset, value, size);
	}

	void Material::setUniform(const std::string& name, const void* value, unsigned int size, const void* offset) 
	{
		for(int i = 0; i < this->getShaderProgram()->getUniforms().size(); i++) 
		{
			if(this->getShaderProgram()->getUniforms()[i].name == name) 
			{
				this->setUniform(i, value, size, offset);
				return;
			}
		}
	}

	unsigned int Material::getUniformIndex(const std::string& name) 
	{
		for(int i = 0; i < this->getShaderProgram()->getUniforms().size(); i++) 
		{
			if(this->getShaderProgram()->getUniforms()[i].name == name) 
			{
				return i;
			}
		}
	}

	void Material::setTexture(const std::string& name, const unsigned int* value, unsigned int size, unsigned int arrayOffset, bool volume) 
	{
		int samplerIndex = 0;
		for(int i = 0; i < program->getSamplers().size(); i++) 
		{
			if(program->getSamplers()[i].name == name) 
			{
				memcpy(&savedSamplers.data()[samplerIndex + arrayOffset], value, sizeof(unsigned int) * size);
				if(volume) for(int j = 0; j < size; j++) 
				{
					savedSamplers[samplerIndex + arrayOffset + j] |= 1 << 31;
				}
				return;
			}
			samplerIndex += program->getSamplers()[i].arraySize;
		}
	}

	void Material::setTextureByIndex(unsigned int index, const unsigned int* value, unsigned int size, unsigned int arrayOffset, bool volume) 
	{
		memcpy(&savedSamplers.data()[index + arrayOffset], value, sizeof(unsigned int) * size);
		if(volume) for(int j = 0; j < size; j++) 
		{
			savedSamplers[index + arrayOffset + j] |= 1 << 31;
		}
	}

	void Material::setSingleTextureByIndex(unsigned int index, unsigned int texture, bool volume) 
	{
		savedSamplers[index] = texture;
		if(volume) savedSamplers[index] |= 1 << 31;
	}
		
	unsigned int Material::getTextureIndex(const std::string& name) const
	{
		int samplerIndex = 0;
		for(int i = 0; i < program->getSamplers().size(); i++) 
		{
			if(program->getSamplers()[i].name == name) 
			{
				return samplerIndex;
			}
			samplerIndex += program->getSamplers()[i].arraySize;
		}
		return  -1;
	}
	void Material::getTextures(const std::string& name, unsigned int* textures, unsigned int size, unsigned int arrayOffset) const
	{
		int samplerIndex = 0;
		for(int i = 0; i < program->getSamplers().size(); i++) 
		{
			if(program->getSamplers()[i].name == name) 
			{
				memcpy(textures, &savedSamplers.data()[samplerIndex + arrayOffset], sizeof(unsigned int) * size);
				return;
			}
			samplerIndex += program->getSamplers()[i].arraySize;
		}
	}

	void Material::setUniformBlock(const std::string& name, unsigned int ubo) 
	{
		for(int i = 0; i < program->getUniformBlocks().size(); i++) 
		{
			if(program->getUniformBlocks()[i].name == name) 
			{
				this->UBOs[i] = ubo;
				break;
			}
		}
	}

	Material::Material(ShaderProgram* program) : program(nullptr) 
	{
		setProgram(program);
	}

	Material::Material() 
	{
		program = nullptr;
	}

	void Material::setProgram(ShaderProgram* program)
	{
		//if(this->program) return;
		this->program = program;
		for(void* u : savedUniforms) 
		{
			delete[] (char*)u;
		}
		savedUniforms.clear();
		savedSamplers.clear();
		
		const auto& uniforms = program->getUniforms();
		for(const auto& uniform : uniforms) 
		{
			savedUniforms.push_back(new char[uniform.byteSize * uniform.arraySize]);
		}
		int samplerCount = 0;
		for(const auto& sampler : program->getSamplers()) 
		{
			samplerCount += sampler.arraySize;
		}
		this->savedSamplers = std::vector<unsigned int>(samplerCount);
		memset(savedSamplers.data(), 0, sizeof(unsigned int) * savedSamplers.size());
		this->UBOs = std::vector<unsigned int>(program->getUniformBlocks().size());
	}

	void Material::useProgram() const
	{
		glUseProgram(program->getId());
	}

	void Material::setupUniforms() const
	{
		const auto& uniforms = program->getUniforms();
		for(int i = 0; i < uniforms.size(); i++) 
		{
			glUniformAuto(uniforms[i].location, uniforms[i].arraySize, savedUniforms[i], uniforms[i].type);
		}
	}

	void Material::setupUniformBlocks(UBOBindings& bindings) const
	{
		for(int i = 0; i < UBOs.size(); i++) 
		{
			bindings.bind(program->getUniformBlocks()[i].binding, UBOs[i]);
		}
	}

	void Material::setupTextures(TextureBindings& bindings) const
	{
		for(unsigned int i = 0; i < savedSamplers.size(); i++) 
		{
			bindings.bind(i, savedSamplers[i]);
		}
	}

	Material::~Material() 
	{
		for(void* data : savedUniforms) 
		{
			delete[] ((char*)data);
		}
	}

	void* Material::getUniformDataPtrByName(const std::string& name) 
	{
		int i = 0;
		for(const Kirino::ShaderProgram::UniformRecord& ur : this->program->getUniforms()) 
		{
			if(ur.name == name) break;
			i++;
		}
		return savedUniforms[i];
	}
	
	unsigned int Material::getSamplerByName(const std::string& name) 
	{
		int i = 0;
		for(const Kirino::ShaderProgram::UniformRecord& ur : this->program->getSamplers()) 
		{
			if(ur.name == name) break;
			i++;
		}
		return savedSamplers[i];
	}
} // namespace Kirino

void glUniformAuto(int location, int arraySize, const void* value, GLenum type) 
{
	switch (type)
	{
	case GL_FLOAT:
		glUniform1fv(location, arraySize, reinterpret_cast<const float*>(value));
		break;
	case GL_FLOAT_VEC2:
		glUniform2fv(location, arraySize, reinterpret_cast<const float*>(value));
		break;
	case GL_FLOAT_VEC3:
		glUniform3fv(location, arraySize, reinterpret_cast<const float*>(value));
		break;
	case GL_FLOAT_VEC4:
		glUniform4fv(location, arraySize, reinterpret_cast<const float*>(value));
		break;
	case GL_INT:
		glUniform1iv(location, arraySize, reinterpret_cast<const int*>(value));
		break;
	case GL_INT_VEC2:
		glUniform2iv(location, arraySize, reinterpret_cast<const int*>(value));
		break;
	case GL_INT_VEC3:
		glUniform3iv(location, arraySize, reinterpret_cast<const int*>(value));
		break;
	case GL_INT_VEC4:
		glUniform4iv(location, arraySize, reinterpret_cast<const int*>(value));
		break;
	case GL_UNSIGNED_INT:
		glUniform1uiv(location, arraySize, reinterpret_cast<const unsigned int*>(value));
		break;
	case GL_UNSIGNED_INT_VEC2:
		glUniform2uiv(location, arraySize, reinterpret_cast<const unsigned int*>(value));
		break;
	case GL_UNSIGNED_INT_VEC3:
		glUniform3uiv(location, arraySize, reinterpret_cast<const unsigned int*>(value));
		break;
	case GL_UNSIGNED_INT_VEC4:
		glUniform4uiv(location, arraySize, reinterpret_cast<const unsigned int*>(value));
		break;
	case GL_FLOAT_MAT2:
		glUniformMatrix2fv(location, arraySize, false, reinterpret_cast<const float*>(value));
		break;
	case GL_FLOAT_MAT3:
		glUniformMatrix3fv(location, arraySize, false, reinterpret_cast<const float*>(value));
		break;
	case GL_FLOAT_MAT4:
		glUniformMatrix4fv(location, arraySize, false, reinterpret_cast<const float*>(value));
		break;
	default:
		break;
	}
}