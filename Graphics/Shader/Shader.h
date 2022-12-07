#pragma once
#include <string>
#include <vector>
#include "Uniform.h"
namespace TealEngine
{
	class Shader
	{
	private:
		unsigned int shader;
		std::vector<Uniform*> uniforms;
	public:

		void loadFromFile(std::string path, unsigned int type);

		void loadFromString(std::string source, unsigned int type);

		unsigned int id();

		~Shader();
	};
}

