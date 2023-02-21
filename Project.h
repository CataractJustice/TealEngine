#pragma once
#include <string>
#include "NlohmannJson/json.hpp"
#include "System/DLLoader.h"

using Json = nlohmann::json;

namespace TealEngine
{
	class Project 
	{
		public:
		Json projectProperties;
		DLLoader dlLoader;

		static Project create(const std::string& directory, const std::string& title);
		static Project open(const std::string& directory);
		void buildLibs();
		void loadLibs();
		std::string error;

		inline std::string getTitle() { return projectProperties["title"]; };
		inline std::string getPath() { return projectProperties["path"]; };
		inline std::string getDefaultScene() { return projectProperties["defaultScene"]; };
		inline void setTitle(const std::string& title) { this->projectProperties["title"] = title; }
		inline void setPath(const std::string& path) { this->projectProperties["path"] = path; }
		inline void setDefaultScene(const std::string& defaultScene) { this->projectProperties["defaultScene"] = defaultScene; }

		void save();
	};
} // namespace TealEngine
