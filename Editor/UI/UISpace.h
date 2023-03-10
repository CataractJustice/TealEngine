#pragma once
#include "libs/imgui/imgui.h"
#include <map>
#include <string>
namespace TealEngine 
{
	namespace EditorWindowNames 
	{
		const std::string gameSceneTree = "Game scene tree";
		const std::string editorSceneTree = "Editor scene tree";
		const std::string gameViewport = "Game viewport";
		const std::string editorViewport = "Editor viewport";
		const std::string textureDebugger = "Texture debugger";	
		const std::string gameNodeProps = "Node properties";
		const std::string componentsExplorer = "Components";
		const std::string gameAssetsBrowser = "Game assets browser";
		const std::string projectProps = "Project properties";
	};
	class UISpace 
	{
		private:
		std::map<std::string, bool> windowOptions;
		bool projectCreateWindowActive;
		std::string projectCreateTitleInput;
		public:
		UISpace();
		//displays file menu, docking space and all currently oppened windows
		void display();
		//adds a window option to the file menu
		void addWindowOption(const std::string& name);
		//opens a ui window with given name
		//does nothing if there's no window option with provided name
		void openWindow(const std::string& name);
		//closes window with given name
		//does nothing if no window with such name is open
		void closeWindow(const std::string& name);
	};
}