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
		std::map<std::string, std::pair<unsigned int, unsigned int>> texture;
		void addTexture(std::string name);
		void tryAddUniform(std::string name) 
		{
			if (uniforms.find(name) == uniforms.cend())
				uniforms[name] = new Uniform(program, name);
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
		}

		ShaderProgram(const ShaderProgram& sp) : ShaderProgram()
		{
			program = sp.program;
			for (auto& upair : sp.uniforms) 
			{
				uniforms[upair.first] = new Uniform(*upair.second);
			}

			this->texture = sp.texture;
		}

		ShaderProgram& operator=(const ShaderProgram& sp) 
		{
			program = sp.program;
			for (auto& upair : sp.uniforms)
			{
				uniforms[upair.first] = new Uniform(*upair.second);
			}

			this->texture = sp.texture;
			return *this;
		}

		ShaderProgram& operator=(ShaderProgram& sp)
		{
			program = sp.program;
			for (auto& upair : sp.uniforms)
			{
				uniforms[upair.first] = new Uniform(*upair.second);
			}

			this->texture = sp.texture;
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

		void setUniform(std::string name, int x)		 {tryAddUniform(name); uniforms[name]->set1iv(&x, 1); }
		void setUniform(std::string name, float x)		 {tryAddUniform(name); uniforms[name]->set1fv(&x, 1); }
		void setUniform(std::string name, glm::vec2 vec) {tryAddUniform(name); uniforms[name]->set2fv(glm::value_ptr(vec), 1); }
		void setUniform(std::string name, glm::vec3 vec) {tryAddUniform(name); uniforms[name]->set3fv(glm::value_ptr(vec), 1); }
		void setUniform(std::string name, glm::vec4 vec) {tryAddUniform(name); uniforms[name]->set4fv(glm::value_ptr(vec), 1); }
		void setUniform(std::string name, glm::mat4 mat) {tryAddUniform(name); uniforms[name]->setm4fv(glm::value_ptr(mat), 1); }

		void setUniform(std::string name, glm::mat4* mat, int size) 
		{ 
			float* matv = new float[16 * size];
			for (int i = 0; i < size; i++)
				memcpy(matv + i * 16, value_ptr(mat[i]), sizeof(float) * 16);
			tryAddUniform(name); uniforms[name]->setm4fv(matv, size);
			delete[] matv;
		}

		void setUniform(std::map<std::string, Uniform*>::iterator it, int x)			{ it->second->set1iv(&x, 1); }
		void setUniform(std::map<std::string, Uniform*>::iterator it, float x)		{ it->second->set1fv(&x, 1); }
		void setUniform(std::map<std::string, Uniform*>::iterator it, glm::vec2 vec) { it->second->set2fv(glm::value_ptr(vec), 1); }
		void setUniform(std::map<std::string, Uniform*>::iterator it, glm::vec3 vec) { it->second->set3fv(glm::value_ptr(vec), 1); }
		void setUniform(std::map<std::string, Uniform*>::iterator it, glm::vec4 vec) { it->second->set4fv(glm::value_ptr(vec), 1); }
		void setUniform(std::map<std::string, Uniform*>::iterator it, glm::mat4 mat) { it->second->setm4fv(glm::value_ptr(mat), 1); }

		auto getUniformIterator(std::string name) 
		{
			return uniforms.find(name);
		}

		unsigned int id();
	};
}