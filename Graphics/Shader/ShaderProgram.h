#pragma once
#include "Shader.h"
#include "Uniform.h"
#include <map>

namespace TealEngine {

	class ShaderProgram
	{
	private:
		unsigned int program;
		static unsigned int lastMaterialId;
		static unsigned int usedMaterialId;
		unsigned int materialId;
		std::map<std::string, Uniform*> uniforms;
		Uniform* uniformsArray[1024];
		std::pair<unsigned int, unsigned int>* textureArray[64];
		std::map<std::string, std::pair<unsigned int, unsigned int>> texture;
		void addTexture(std::string name);
		void tryAddUniform(std::string name) 
		{
			if (uniforms.find(name) == uniforms.cend()) 
			{
				Uniform* uni = new Uniform(program, name);
				uniformsArray[uniforms.size()] = uni;
				uniforms[name] = uni;
			}
		}
	public:

		ShaderProgram() 
		{
			materialId = lastMaterialId++;
		}

		ShaderProgram(ShaderProgram& sp) : ShaderProgram()
		{
			program = sp.program;
			for (auto& upair : sp.uniforms)
			{
				uniforms[upair.first] = new Uniform(*upair.second);
			}

			this->texture = sp.texture;
			int i = 0;
			for (auto& t : texture)
			{
				textureArray[i] = &t.second;
				i++;
			}
		}

		ShaderProgram(const ShaderProgram& sp) : ShaderProgram()
		{
			program = sp.program;
			for (auto& upair : sp.uniforms) 
			{
				uniforms[upair.first] = new Uniform(*upair.second);
			}

			this->texture = sp.texture; 
			int i = 0;
			for (auto& t : texture)
			{
				textureArray[i] = &t.second;
				i++;
			}
		}

		ShaderProgram& operator=(const ShaderProgram& sp) 
		{
			program = sp.program;
			for (auto& upair : sp.uniforms)
			{
				Uniform* uni = new Uniform(*upair.second);
				uniformsArray[uniforms.size()] = uni;
				uniforms[upair.first] = uni;
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

		ShaderProgram& operator=(ShaderProgram& sp)
		{
			program = sp.program;
			for (auto& upair : sp.uniforms)
			{
				Uniform* uni = new Uniform(*upair.second);
				uniformsArray[uniforms.size()] = uni;
				uniforms[upair.first] = uni;
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

		~ShaderProgram() 
		{
			//for (std::pair<std::string, std::pair<unsigned short, Uniform*>> u : uniform)
			//	delete u.second.second;
		}

		void loadFromFiles(std::string vertpath, std::string fragpath);

		void link(unsigned int vertexShader, unsigned int fragmentShader);

		void link(Shader vertexShader, Shader geometryShader, Shader fragmentShader);

		void use();

		void setTexture(std::string name, unsigned int texture);
		void setTexture(std::string name, unsigned int index, unsigned int texture);

		void setUniform(std::string name, int x)		 { if (uniforms.find(name) != uniforms.cend()) uniforms[name]->set1iv(&x, 1); }
		void setUniform(std::string name, float x)		 { if (uniforms.find(name) != uniforms.cend()) uniforms[name]->set1fv(&x, 1); }
		void setUniform(std::string name, glm::vec2 vec) { if (uniforms.find(name) != uniforms.cend()) uniforms[name]->set2fv(glm::value_ptr(vec), 1); }
		void setUniform(std::string name, glm::vec3 vec) { if (uniforms.find(name) != uniforms.cend()) uniforms[name]->set3fv(glm::value_ptr(vec), 1); }
		void setUniform(std::string name, glm::vec4 vec) { if (uniforms.find(name) != uniforms.cend()) uniforms[name]->set4fv(glm::value_ptr(vec), 1); }
		void setUniform(std::string name, glm::mat4 mat) { if (uniforms.find(name) != uniforms.cend()) uniforms[name]->setm4fv(glm::value_ptr(mat), 1); }

		void setUniform(std::string name, glm::mat4* mat, int size) 
		{ 
			float* matv = new float[long(16 * size)];
			for (int i = 0; i < size; i++)
				memcpy(matv + i * 16, value_ptr(mat[i]), sizeof(float) * 16);
			uniforms[name]->setm4fv(matv, size);
			delete[] matv;
		}

		void setUniform(std::map<std::string, Uniform*>::iterator it, int x)		 { if (it != uniforms.cend()) it->second->set1iv(&x, 1); }
		void setUniform(std::map<std::string, Uniform*>::iterator it, float x)		 { if (it != uniforms.cend()) it->second->set1fv(&x, 1); }
		void setUniform(std::map<std::string, Uniform*>::iterator it, glm::vec2 vec) { if (it != uniforms.cend()) it->second->set2fv(glm::value_ptr(vec), 1); }
		void setUniform(std::map<std::string, Uniform*>::iterator it, glm::vec3 vec) { if (it != uniforms.cend()) it->second->set3fv(glm::value_ptr(vec), 1); }
		void setUniform(std::map<std::string, Uniform*>::iterator it, glm::vec4 vec) { if (it != uniforms.cend()) it->second->set4fv(glm::value_ptr(vec), 1); }
		void setUniform(std::map<std::string, Uniform*>::iterator it, glm::mat4 mat) { if (it != uniforms.cend()) it->second->setm4fv(glm::value_ptr(mat), 1); }

		auto getUniformIterator(std::string name) 
		{
			return uniforms.find(name);
		}

		unsigned int id();
	};
}