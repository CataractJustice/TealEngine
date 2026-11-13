#pragma once
#include <map>
#include <string>
namespace TealEngine
{
	class LaunchArguments 
	{
		private:
		std::map<std::string, std::string> args;
		public:
		void parse(int argc, char** argv);
		const std::string& get(const std::string& arg) const;
		const bool exists(const std::string& arg) const;
	};
} // namespace TealEngine
