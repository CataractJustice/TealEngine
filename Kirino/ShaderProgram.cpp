#include <GL/glew.h>
#include "ShaderProgram.h"
#include "ShaderCompiler.h"
#include "UniformTypeToByteSize.hpp"
#include <iostream>
#include <vector>
//#define TestLogUniforms

namespace Kirino
{
	ShaderProgram::ShaderProgram() : id(0)
	{

	}

	ShaderProgram::ShaderProgram(unsigned int vert, unsigned int frag) : id(0)
	{
		link(vert, frag);
	}

	ShaderProgram::~ShaderProgram() 
	{
		if(id) 
		{
			//delete id
			glDeleteProgram(id);
		}
	}

	void ShaderProgram::link(unsigned int vert, unsigned int frag) 
	{
		if(id) 
		{
			throw "Linked id twice, shouldn't happen.";
		}
		ShaderCompiler::link(vert, frag, id);

		GLint uniformCount = 0;
		glUseProgram(id);
		glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniformCount);
		#ifdef TestLogUniforms
		std::cout << "\nactive uniforms:\t " << uniformCount << "\n";
		#endif
		
		if(uniformCount != 0) 
		{
			GLint uniformMaxNameLength = 0;
			GLint uniformArraySize = 0;
			GLint uniformNamelength = 0;
			GLenum uniformType = 0;
			
		
			glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformMaxNameLength);
			char* uniformNameBuffer = new char[uniformMaxNameLength];
			int samplerCount = 0;
			for (int i = 0; i < uniformCount; i++)
			{
				glGetActiveUniform(id, i, uniformMaxNameLength, &uniformNamelength, &uniformArraySize, &uniformType, uniformNameBuffer);
				GLint location = glGetUniformLocation(id, uniformNameBuffer);
				
				if(location == -1) continue;
				
				if(
				uniformType == GL_SAMPLER_1D || 
				uniformType == GL_SAMPLER_2D || 
				uniformType == GL_SAMPLER_3D || 
				uniformType == GL_UNSIGNED_INT_SAMPLER_1D || 
				uniformType == GL_UNSIGNED_INT_SAMPLER_2D || 
				uniformType == GL_UNSIGNED_INT_SAMPLER_3D ) 
				{
					samplerMap[uniformNameBuffer] = samplers.size();
					this->samplers.push_back({samplerCount, uniformType, uniformArraySize, uniformTypeToByteSize(uniformType), uniformNameBuffer});
					int* samplersValue = new int[samplers.back().arraySize];
					for(int s = 0; s < samplers.back().arraySize; s++) 
					{
						samplersValue[s] = samplerCount;
						samplerCount++;
					}
					glUniform1iv(location, samplers.back().arraySize, samplersValue);
					delete[] samplersValue;
				}
				else 
				{
					this->uniforms.push_back({location, uniformType, uniformArraySize, uniformTypeToByteSize(uniformType), uniformNameBuffer});
				}

				#ifdef TestLogUniforms
				std::cout << "#" << i << "\n"
				<< "uniform name:\t " << uniformNameBuffer << "\n"
				<< "uniform location:\t " << location << "\n"
				<< "uniform array size:\t " << uniformArraySize << "\n"
				<< "uniform type:\t " << uniformType << "\n";
				#endif
			}
			delete[] uniformNameBuffer;
		}
		GLint uniformBlockCount = 0;
		glGetProgramiv(id, GL_ACTIVE_UNIFORM_BLOCKS, &uniformBlockCount);
		#ifdef TestLogUniforms
		std::cout << "\nuniform block count:\t " << uniformBlockCount << "\n";
		#endif
		if (uniformBlockCount != 0) 
		{
			GLint uniformBlockMaxNameLength = 0;
			glGetProgramiv(id, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &uniformBlockMaxNameLength);
			char* uniformBlockNameBuffer = new char[uniformBlockMaxNameLength];

			for (GLint i = 0; i < uniformBlockCount; ++i) 
			{
				GLint blockBinding = i;
				GLint blockNamelength = 0;
				GLint blockDataSize = 0;
				GLint activeUniforms = 0;
				glGetActiveUniformBlockName(id, i, uniformBlockMaxNameLength, &blockNamelength, uniformBlockNameBuffer);
				glUniformBlockBinding(id, i, blockBinding);
				glGetActiveUniformBlockiv(id, i, GL_UNIFORM_BLOCK_DATA_SIZE, &blockDataSize);
				glGetActiveUniformBlockiv(id, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &activeUniforms);

				std::vector<GLint> uniformIndices(activeUniforms);
				glGetActiveUniformBlockiv(id, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, uniformIndices.data());
				#ifdef TestLogUniforms
				std::cout << "block name:\t " << uniformBlockNameBuffer << "\n"
				<< "block binding:\t " << blockBinding << "\n"
				<< "block data size:\t " << blockDataSize << "\n"
				<< "block active uniforms:\t " << activeUniforms << "\n";
				#endif
				uniformBlocks.push_back({});
				UniformBlockRecord& uniformBlock = uniformBlocks.back();
				for (GLint j = 0; j < activeUniforms; ++j) {
					GLuint uniformIndex = uniformIndices[j];
					GLint uniformNameLength = 0;
					GLsizei nameLength = 0;
					GLint uniformType = 0;
					GLint uniformSize = 0;
					glGetActiveUniformsiv(id, 1, &uniformIndex, GL_UNIFORM_NAME_LENGTH, &uniformNameLength);
					std::vector<char> nameBuffer(uniformNameLength);
					glGetActiveUniformName(id, uniformIndex, uniformNameLength, &nameLength, nameBuffer.data());
					glGetActiveUniformsiv(id, 1, &uniformIndex, GL_UNIFORM_TYPE, &uniformType);
					glGetActiveUniformsiv(id, 1, &uniformIndex, GL_UNIFORM_SIZE, &uniformSize);
					#ifdef TestLogUniforms
					std::cout << "block uniform index:\t " << uniformIndex << "\n"
					<< "block uniform name:\t " << nameBuffer.data() << "\n"
					<< "block uniform array size:\t " << uniformSize << "\n"
					<< "block uniform type:\t " << uniformType << "\n";
					#endif
					uniformBlock.uniforms.push_back({uniformType, uniformSize, uniformTypeToByteSize(uniformType)});
				}
				uniformBlock.byteSize = blockDataSize;
				uniformBlock.binding = blockBinding;
				uniformBlock.name = uniformBlockNameBuffer;
			}
			delete[] uniformBlockNameBuffer;
		}
	}

	void ShaderProgram::use() 
	{
		static int usedProgram = 0;
		if(usedProgram == id) 
		{
			throw "Same shader id used twice in a row, shouldn't happen, seems like a skill issue, get crashed.";
		}
		usedProgram = id;
		glUseProgram(id);
	}

	const ShaderProgram::UniformRecord* ShaderProgram::getUniformRecordByName(const std::string& name) const
	{
		for(const UniformRecord& ur : uniforms) 
		{
			if(ur.name == name) 
			{
				return &ur;
			}
		}
		return nullptr;
	}

	const ShaderProgram::UniformRecord* ShaderProgram::getSamplerUniformRecordByName(const std::string& name) const 
	{
		for(const UniformRecord& ur : samplers) 
		{
			if(ur.name == name) 
			{
				return  &ur;
			}
		}
		return nullptr;
	}

	bool ShaderProgram::hasSampler(const std::string& name) 
	{
		return this->samplerMap.find(name) != this->samplerMap.cend();
	}
} // namespace Kirino
