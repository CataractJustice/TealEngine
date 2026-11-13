#pragma once
#include <map>
#include <vector>
#include <string>
namespace Kirino
{
	class ShaderProgram 
	{
		private:
		unsigned int id;
		public:
		struct UniformRecord 
		{
			int location;
			unsigned int type;
			int arraySize;
			unsigned int byteSize;
			std::string name;
		};
		struct UniformBlockUniformRecord 
		{
			int type;
			int arraySize;
			unsigned int byteSize;
			std::string name;
		};
		struct UniformBlockRecord 
		{
			std::vector<UniformBlockUniformRecord> uniforms;
			int byteSize;
			int binding;
			std::string name;
		};
		private:
		std::vector<UniformRecord> uniforms;
		std::vector<UniformRecord> samplers;
		std::vector<UniformBlockRecord> uniformBlocks;
		std::map<std::string, int> samplerMap;
		public:
		inline unsigned int getId() const { return id; }
		inline const std::vector<UniformRecord>& getUniforms() const
		{
			return uniforms;
		}
		inline const std::vector<UniformRecord>& getSamplers() const 
		{
			return samplers;
		}
		inline const std::vector<UniformBlockRecord>& getUniformBlocks() const 
		{
			return uniformBlocks;
		}
		const UniformRecord* getUniformRecordByName(const std::string& name) const;
		const UniformRecord* getSamplerUniformRecordByName(const std::string& name) const;
		bool hasSampler(const std::string& name);

		ShaderProgram();
		ShaderProgram(unsigned int vert, unsigned int frag);
		void link(unsigned int vert, unsigned int frag);
		void use();
		~ShaderProgram();
	};
} // namespace Kirino
