#pragma once
#include "Shader.h"
#include "Uniform.h"
#include <map>
#include <filesystem>

namespace TealEngine {

	class ShaderProgram
	{
	private:
		unsigned int program;
		static unsigned int lastMaterialId;
		static unsigned int usedMaterialId;
		unsigned int materialId;
		std::map<std::string, Uniform> uniforms;
		std::pair<unsigned int, unsigned int>* textureArray[64];
		std::map<std::string, std::pair<unsigned int, unsigned int>> texture;
		void addTexture(std::string name);
		void tryAddUniform(std::string name, int type);
	public:

		ShaderProgram();

		ShaderProgram(const ShaderProgram& sp);
		ShaderProgram& operator=(const ShaderProgram& sp);

		~ShaderProgram();

		void loadFromFiles(std::string vertpath, std::string fragpath);

		void link(unsigned int vertexShader, unsigned int fragmentShader);

		void link(Shader vertexShader, Shader geometryShader, Shader fragmentShader);

		void use();

		void setTexture(std::string name, unsigned int texture);
		void setTexture(std::string name, unsigned int index, unsigned int texture);

		inline void setUniform(std::string name, int x)		 { if (uniforms.find(name) != uniforms.cend()) uniforms[name].set1iv(&x, 1); }
		inline void setUniform(std::string name, float x)		 { if (uniforms.find(name) != uniforms.cend()) uniforms[name].set1fv(&x, 1); }
		inline void setUniform(std::string name, glm::vec2 vec) { if (uniforms.find(name) != uniforms.cend()) uniforms[name].set2fv(glm::value_ptr(vec), 1); }
		inline void setUniform(std::string name, glm::vec3 vec) { if (uniforms.find(name) != uniforms.cend()) uniforms[name].set3fv(glm::value_ptr(vec), 1); }
		inline void setUniform(std::string name, glm::vec4 vec) { if (uniforms.find(name) != uniforms.cend()) uniforms[name].set4fv(glm::value_ptr(vec), 1); }
		inline void setUniform(std::string name, glm::mat4 mat) { if (uniforms.find(name) != uniforms.cend()) uniforms[name].setm4fv(glm::value_ptr(mat), 1); }

		void setUniform(std::string name, glm::mat4* mat, int size);
		inline void setUniform(std::map<std::string, Uniform>::iterator it, int x)		 { if (it != uniforms.cend()) it->second.set1iv(&x, 1); }
		inline void setUniform(std::map<std::string, Uniform>::iterator it, float x)		 { if (it != uniforms.cend()) it->second.set1fv(&x, 1); }
		inline void setUniform(std::map<std::string, Uniform>::iterator it, glm::vec2 vec) { if (it != uniforms.cend()) it->second.set2fv(glm::value_ptr(vec), 1); }
		inline void setUniform(std::map<std::string, Uniform>::iterator it, glm::vec3 vec) { if (it != uniforms.cend()) it->second.set3fv(glm::value_ptr(vec), 1); }
		inline void setUniform(std::map<std::string, Uniform>::iterator it, glm::vec4 vec) { if (it != uniforms.cend()) it->second.set4fv(glm::value_ptr(vec), 1); }
		inline void setUniform(std::map<std::string, Uniform>::iterator it, glm::mat4 mat) { if (it != uniforms.cend()) it->second.setm4fv(glm::value_ptr(mat), 1); }

		std::map<std::string, Uniform>::iterator  getUniformIterator(std::string name);
		
		inline std::map<std::string, Uniform>& getUniformsMap() 
		{
			return this->uniforms;
		}

		inline std::map<std::string, std::pair<unsigned int, unsigned int>>& getTextureMap() 
		{
			return this->texture;
		}

		unsigned int id() const;

		//loads shaders into
		static void loadShadersFromJson(std::map<std::string, ShaderProgram>& shadersMap, const std::string& linkingJsonFilePath);

		void saveMaterial(const std::filesystem::path& path, const std::string& name);
	};
}