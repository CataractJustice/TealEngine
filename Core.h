#pragma once
#include <map>
#include <string>
#include "Resources.h"
#include "System/Input.h"
#include "Graphics/Renderer/DeferredRenderer.h"
#include "Graphics/Renderer/IdRenderer.h"
#include "GameNode/GameNode3D.h"
#include "Editor/ActionList.h"
#include "Editor/UI/UISpace.h"
#include "Project.h"
namespace TealEngine 
{
	namespace Core 
	{
		extern DeferredRenderer renderer;
		extern IdRenderer idRenderer;
		extern ActionList actionList;
		extern Project currentProject;
		extern UISpace uiSpace;

		void update();
		void setProject(Project project);
		GameNode3D* getRoot();
		extern std::map<std::string, std::string> settings;
		void init();
		bool isServer();
		void requestModulesReload();
		void setScene(GameNode3D* node);

		void play();
		void pause();
		void stop();
	}
}