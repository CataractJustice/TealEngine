#pragma once
#include "Shader.h"
#include "Uniform.h"
#include <map>

namespace TealEngine {

	class ShaderProgram
	{
	private:
		GLuint program;
		static GLuint lastMaterialId;
		static GLuint usedMaterialId;
		GLuint materialId;
		std::map<std::string, std::pair<unsigned short, Uniform*>> uniform;
		std::map<std::string, std::pair<GLuint, GLuint>> texture;
		void addTexture(std::string name);
	public:
		~ShaderProgram() 
		{
			for (std::pair<std::string, std::pair<unsigned short, Uniform*>> u : uniform)
				delete u.second.second;
		}

		void loadFromFiles(std::string vertpath, std::string fragpath);

		void link(GLuint vertexShader, GLuint fragmentShader);

		void link(Shader vertexShader, Shader geometryShader, Shader fragmentShader);

		void use();

		template <class T>
		void addUniform(std::string name)
		{
			uniform[name].second = new TemplateUniform<T>(program, name);
		}

		void setTexture(std::string name, GLuint texture);
		void setTexture(std::string name, GLuint index, GLuint texture);

		std::map<std::string, std::pair<unsigned short, Uniform*>>::iterator getUniformIterator(std::string name)
		{
			return uniform.find(name);
		}

		template <class T>
		void setUniform(std::string id, T* value, int length = 1);

		template <class T>
		void sharedSetUniform(std::string id, T* value, int length = 1);

		template <class T>
		void setUniform(std::map<std::string, std::pair<unsigned short, Uniform*>>::iterator it, T* value, int length = 1);

		template <class T>
		void sharedSetUniform(std::map<std::string, std::pair<unsigned short, Uniform*>>::iterator it, T* value, int length = 1);

		GLuint id();

		ShaderProgram clone() 
		{
			ShaderProgram s(*this);
			for (std::pair<std::string, std::pair<bool, Uniform*>> u : s.uniform)
			{
				u.second.second = u.second.second->clone();
			}
			lastMaterialId++;
			materialId = lastMaterialId;
			return s;
		}
	};


	template <class T>
	void ShaderProgram::setUniform(std::string id, T* value, int length)
	{
		if (uniform.find(id) == uniform.end())
		{
			addUniform<T>(id);
			setUniform<T>(id, value, length);
		}
		else
			if (dynamic_cast<TemplateUniform<T>*>(uniform[id].second))
				((TemplateUniform<T>*)uniform[id].second)->set(value, length);
			else
				throw ("Types of setting and uniform values is not the same.");
		uniform[id].first = 1;
	}

	template <class T>
	void ShaderProgram::sharedSetUniform(std::string id, T* value, int length)
	{
		if (uniform.find(id) == uniform.end())
		{
			addUniform<T>(id);
			sharedSetUniform<T>(id, value, length);
		}
		else
			if (dynamic_cast<TemplateUniform<T>*>(uniform[id]))
				((TemplateUniform<T>*)uniform[id])->sharedSet(value, length);
			else
				throw ("Types of setting and uniform values is not the same.");
		uniform[id].first = 2;
	}

	template <class T>
	void ShaderProgram::setUniform(std::map<std::string, std::pair<unsigned short, Uniform*>>::iterator it, T* value, int length)
	{
		if (dynamic_cast<TemplateUniform<T>*>((*it).second.second))
			((TemplateUniform<T>*)(*it).second.second)->set(value, length);
		else
			throw ("Types of setting and uniform values is not the same.");
		(*it).second.first = 1;
	}

	template <class T>
	void ShaderProgram::sharedSetUniform(std::map<std::string, std::pair<unsigned short, Uniform*>>::iterator it, T* value, int length)
	{
		if (dynamic_cast<TemplateUniform<T>*>((*it).second.second))
			((TemplateUniform<T>*)(*it).second.second)->sharedSet(value, length);
		else
			throw ("Types of setting and uniform values is not the same.");
		(*it).second.first = 2;
	}
}