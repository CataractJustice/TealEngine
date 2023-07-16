#define GLEW_STATIC
#include "ShaderProgram.h"
#include "FileSystem/loadStrFromFile.h"
#include "Graphics/Graphics.h"
#include "System/Debug.h"
#include <iostream>
#include <fstream>
#include "NlohmannJson/json.hpp"
#include "Core.h"
using Json = nlohmann::json;
namespace TealEngine {

	GLuint ShaderProgram::lastMaterialId = 0;
	GLuint ShaderProgram::usedMaterialId = 0;

	void ShaderProgram::loadFromFiles(std::string vertpath, std::string fragpath) 
	{
		Shader vert;
		vert.loadFromFile(vertpath, GL_VERTEX_SHADER);
		Shader frag;
		frag.loadFromFile(fragpath, GL_FRAGMENT_SHADER);
		this->link(vert.id(), frag.id());
	}

	void ShaderProgram::addTexture(std::string name)
	{
		glUseProgram(this->program);
		GLuint loc = glGetUniformLocation(this->program, name.c_str());
		if (loc == -1) TE_DEBUG_WARNING("Could not find uniform with name \"" + name + "\" in shader.");
		glUniform1i(loc, this->texture.size());
		this->texture[name] = std::pair<GLuint, GLuint>(texture.size(), 0);
		textureArray[texture.size() - 1] = &texture[name];
	}

	void ShaderProgram::tryAddUniform(std::string name, int type) 
		{
			if (uniforms.find(name) == uniforms.cend()) 
			{
				uniforms[name] = Uniform(program, name, type);
			}
		}

	void ShaderProgram::setTexture(std::string name, GLuint texture)
	{
		if (this->texture.find(name) == this->texture.end())
			addTexture(name);
		this->texture[name].second = texture;
	}

	void ShaderProgram::setTexture(std::string name, GLuint index, GLuint texture)
	{
		if (this->texture.find(name + "[" + std::to_string(index) + "]") == this->texture.end())
			addTexture(name + "[" + std::to_string(index) + "]");
		this->texture[name + "[" + std::to_string(index) + "]"].second = texture;
	}

	void ShaderProgram::link(GLuint vertexShader, GLuint fragmentShader)
	{
		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader); 
		glLinkProgram(program);
		GLint success;
		GLchar* infoLog = new GLchar[1024];
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cout << infoLog;
		}
		delete[] infoLog;
		
		GLint uniformsCount;
		GLint buffersize;
		GLint arraysize;
		GLint namelength;
		GLenum type;
		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniformsCount);
		glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &buffersize);
		char* namebuffer = new char[buffersize];
		for (int i = 0; i < uniformsCount; i++)
		{
			glGetActiveUniform(program, i, buffersize, &namelength, &arraysize, &type, namebuffer);
			if (namebuffer[namelength - 1] == ']')
				namebuffer[namelength - 3] = 0; //getting rid of "[0]" from the end of an uniform name
			if(type == GL_SAMPLER_2D)
				addTexture(namebuffer);
			else
				tryAddUniform(namebuffer, type);
		}
		delete[] namebuffer;
		
		lastMaterialId++;
		materialId = lastMaterialId;
	}

	void ShaderProgram::link(Shader vertexShader, Shader geometryShader, Shader fragmentShader)
	{
		program = glCreateProgram();
		glAttachShader(program, vertexShader.id());
		glAttachShader(program, fragmentShader.id());
		glAttachShader(program, geometryShader.id());
		glLinkProgram(program);
		lastMaterialId++;
		materialId = lastMaterialId;
	}

	void ShaderProgram::use()
	{
		static GLuint currentProgram = -1;
		static GLuint bindedTextures[64];

		glUseProgram(program);

		currentProgram = program;
		usedMaterialId = materialId;

		for (int i = 0; i < texture.size(); i++)
		{
			if (bindedTextures[textureArray[i]->first] != textureArray[i]->second) 
			{
				glActiveTexture(GL_TEXTURE0 + textureArray[i]->first);
				glBindTexture(GL_TEXTURE_2D, textureArray[i]->second);
				bindedTextures[textureArray[i]->first] = textureArray[i]->second;
			}
		}
		
		for (auto& upair : uniforms)
		{
			upair.second.use();
		}
	}

	GLuint ShaderProgram::id() const
	{
		return program;
	}

	ShaderProgram::ShaderProgram() 
	{
		materialId = lastMaterialId++;
		this->program = 0;
	}

	ShaderProgram::ShaderProgram(const ShaderProgram& sp) : ShaderProgram()
	{
		program = sp.program;
		uniforms.clear();
		for (auto& upair : sp.uniforms) 
		{
			uniforms[upair.first] = upair.second;
		}
		
		this->texture = sp.texture; 
		int i = 0;
		for (auto& t : texture)
		{
			textureArray[i] = &t.second;
			i++;
		}
	}

	ShaderProgram& ShaderProgram::operator=(const ShaderProgram& sp) 
	{
		program = sp.program;
		uniforms.clear();
		for (auto& upair : sp.uniforms)
		{
			uniforms[upair.first] = upair.second;
		}
		this->texture = sp.texture;
		int i = 0;
		for (auto& t : texture)
		{
			textureArray[i] = &t.second;
			i++;
		}
		return *this;
	}

	ShaderProgram::~ShaderProgram() 
	{
		//for (std::pair<std::string, std::pair<unsigned short, Uniform*>> u : uniform)
		//	delete u.second.second;
	}
	
	void ShaderProgram::setUniform(std::string name, glm::mat4* mat, int size) 
	{ 
		float* matv = new float[long(16 * size)];
		for (int i = 0; i < size; i++)
			memcpy(matv + i * 16, value_ptr(mat[i]), sizeof(float) * 16);
		uniforms[name].setm4fv(matv, size);
		delete[] matv;
	}
	
	std::map<std::string, Uniform>::iterator ShaderProgram::getUniformIterator(std::string name) 
	{
		return uniforms.find(name);
	}

	void ShaderProgram::loadShadersFromJson(std::map<std::string, ShaderProgram>& shadersMap, const std::string& linkingJsonFilePath) 
	{
		std::ifstream jsonFile(linkingJsonFilePath);
		Json json = Json::parse(jsonFile);
		for(auto& shaderJson : json.items()) 
		{
			if(shadersMap.find(shaderJson.key()) != shadersMap.cend()) continue;

			std::string folderPath = linkingJsonFilePath.substr(0, linkingJsonFilePath.find_last_of('/'));
			Shader vert, frag;
			vert.loadFromFile(folderPath + "/" + (std::string)shaderJson.value()["vert"], GL_VERTEX_SHADER);
			frag.loadFromFile(folderPath + "/" + (std::string)shaderJson.value()["frag"], GL_FRAGMENT_SHADER);
			shadersMap[shaderJson.key()] = ShaderProgram();
			shadersMap[shaderJson.key()].link(vert.id(), frag.id());
		}
	}

	void ShaderProgram::saveMaterial(const std::filesystem::path& path, const std::string& name) 
	{
		std::string shaderName = "";
		for(const auto& shader : Core::shadersManager.getLoadedMap()) 
		{
			if(shader.second.id() == this->id()) 
			{
				shaderName = shader.first;
			}
		}

		if(shaderName.length() == 0) return;
		
		Json materialJson = Json();
		materialJson["name"] = name;
		materialJson["shader"] = shaderName;

		std::vector<Json> uniforms;
		std::vector<Json> textures;

		for(const auto& uniform : this->getUniformsMap()) 
		{
			Json uniformJson;
			uniformJson["name"] = uniform.first;
			switch (uniform.second.getType())
			{
			case GL_FLOAT:
				uniformJson["value"] = *((float*)uniform.second.valueptr());
				uniformJson["type"] = "float";
				uniforms.push_back(uniformJson);
				break;
			case GL_INT: 
				uniformJson["value"] = *((int*)uniform.second.valueptr());
				uniformJson["type"] = "int";
				uniforms.push_back(uniformJson);
				break;
			case GL_FLOAT_VEC2:
				uniformJson["value"] = std::vector<float>({(((float*)uniform.second.valueptr())[0]), (((float*)uniform.second.valueptr())[1])});
				uniformJson["type"] = "vec2";
				uniforms.push_back(uniformJson);
				break;
			case GL_FLOAT_VEC3:
				uniformJson["value"] = std::vector<float>({(((float*)uniform.second.valueptr())[0]), (((float*)uniform.second.valueptr())[1]), (((float*)uniform.second.valueptr())[2])});
				uniformJson["type"] = "vec3";
				uniforms.push_back(uniformJson);
				break;
			case GL_FLOAT_VEC4:
				uniformJson["value"] = std::vector<float>({(((float*)uniform.second.valueptr())[0]), (((float*)uniform.second.valueptr())[1]), (((float*)uniform.second.valueptr())[2]), (((float*)uniform.second.valueptr())[3])});
				uniformJson["type"] = "vec4";
				uniforms.push_back(uniformJson);
				break;
			default:
				break;
			}
		}

		materialJson["uniforms"] = uniforms;
		for(const auto& texture : this->getTextureMap()) 
		{
			Json textureJson;
			textureJson["name"] = texture.first;

			for(const auto& textureResource : Core::textureManager.getLoadedMap()) 
			{
				if(texture.second.second == textureResource.second.id()) 
				{
					textureJson["texture"] = textureResource.first;
				}
			}	
			textures.push_back(textureJson);
		}

		materialJson["textures"] = textures;

		std::ofstream outputFile(path);
		outputFile << materialJson;
	}
}