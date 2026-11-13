#pragma once
namespace Kirino 
{
	class ShaderCompiler 
	{
		public:
		static void compile(const char* code, unsigned int type, unsigned int& shader);
		static void link(unsigned int vert, unsigned int frag, unsigned int& program);
	};
}