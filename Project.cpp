#include "Project.h"
#include <fstream>
#include <string>
#include "GameNode/GameNode3D.h"
namespace TealEngine
{
	Project Project::open(const std::string& directory) 
	{
		Project project;
		std::ifstream propsFile(directory + "/props.json");
		project.projectProperties = Json::parse(propsFile);
		return project;
	}

	Project Project::create(const std::string& directory, const std::string& title) 
	{
		Project project;
		std::filesystem::path path(directory);
		path = path/title;
		
		if(std::filesystem::is_directory(path)) 
		{

		}
		else if(!std::filesystem::is_regular_file(path)) 
		{
			std::error_code ec;
			if(!std::filesystem::create_directories(path, ec)) 
			{
				project.error = ec.message();
			}
		}
		else 
		{
			project.error = "Path is a regular file";
			return project;
		}
		
		std::filesystem::copy(std::filesystem::current_path()/"Assets", path/"Assets", std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
		std::filesystem::create_directory(path/"GameAssets");
		std::filesystem::create_directory(path/"build");

		project.projectProperties["title"] = title;
		project.projectProperties["path"] = path.string();

		GameNode3D* defaultScene = new GameNode3D();
		defaultScene->rename("Root");
		std::ofstream defaultSceneFile(path/"GameAssets"/"Root.node");
		defaultSceneFile << defaultScene->toJson();
		defaultSceneFile.close();
		delete defaultScene;

		project.projectProperties["defaultScene"] = (path/"GameAssets/Root.node").string();
		
		std::filesystem::create_directory_symlink(std::filesystem::current_path(), path/"EngineLink");

		std::string CMakeListsTxt = 
		"cmake_minimum_required(VERSION 3.22.1)\n\n"
		"project(" + title + ")\n\n"
		"set(CMAKE_CXX_STANDARD 20)\n"
		"set(CMAKE_CXX_STANDARD_REQUIRED True)\n\n"
		"file(GLOB_RECURSE SOURCES GameAssets/*.cpp)\n\n"
		"add_library(${PROJECT_NAME} SHARED ${SOURCES})\n"
		"target_include_directories(${PROJECT_NAME} PUBLIC ./EngineLink)\n";

		std::ofstream projectPropsFile(path/"props.json");
		std::ofstream projectCMakeFile(path/"CMakeLists.txt");
		projectPropsFile << project.projectProperties;
		projectCMakeFile << CMakeListsTxt;

		return project;
	}

	void Project::buildLibs() 
	{
		system((std::string("cd ") + this->projectProperties["path"].get<std::string>() + std::string("/build && cmake .. && make")).c_str());
	}

	void Project::loadLibs() 
	{
		this->dlLoader.unloadAll();
		this->dlLoader.loadDir(this->projectProperties["path"].get<std::string>() + "/build");
	}

	void Project::save() 
	{
		std::ofstream projectPropsFile(this->getPath()+"/props.json");
		projectPropsFile << projectProperties;
	}
} // namespace TealEngine