#pragma once
namespace Kirino
{
	class Shader 
	{
		private:
		unsigned int id;
		public:
		Shader();
		Shader(const char* source, unsigned int type);
		void compile(const char* source, unsigned int type);
		inline unsigned int getId() const { return id; }
		~Shader();
	};
} // namespace Kirino
