#include "Arguments.h"
#include <iostream>
#include <format>
namespace TealEngine
{
	void LaunchArguments::parse(int argc, char** argv) 
	{
		args.clear();
		if(argc <= 1) return;
		for(int i = 1; i < argc; i++) 
		{
			char* arg = argv[i];
			if(arg[0] != '-') 
			{
				std::cerr << std::format("Stray value \"{}\" at position #{}", arg, i);
			}
			arg++;
			if(arg[0] == '-') 
			{
				arg++;
				if(i + 1 < argc && argv[i + 1][0] != '-') 
				{
					args[arg] = argv[i + 1];
					i++;
					continue;
				}
				args[arg] = "";
				continue;
			}
			if(arg[1] || i + 1 >= argc || argv[i + 1][0] == '-') 
			{
				while(arg[0]) 
				{
					args[arg] = "";
					arg++;
					continue;
				}
			}
			args[arg] = argv[i + 1];
			i++;
		}
	}
	const std::string& LaunchArguments::get(const std::string& arg) const 
	{
		return args.at(arg);
	}
	const bool LaunchArguments::exists(const std::string& arg) const 
	{
		return args.find(arg) != args.cend();
	}
} // namespace TealEngine
