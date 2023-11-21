#pragma once
#include <map>
#include <string>
#include "System/Input.h"
#include "Graphics/Renderer/DeferredRenderer.h"
#include "Graphics/Renderer/IdRenderer.h"
#include "GameNode/GameNode3D.h"
#include "Editor/ActionList.h"
#include "Editor/UI/UISpace.h"
#include "Project.h"
#include "ResourceManagment/TextureManager.h"
#include "ResourceManagment/ModelsManager.h"
#include "ResourceManagment/ShadersManager.h"
#include "ResourceManagment/MaterialsManager.h"
#include "Physics/PhysicsScene.h"
#include "Graphics/Renderer/ShapesRenderer.h"
namespace TealEngine 
{
	namespace Core 
	{
		extern DeferredRenderer renderer;
		extern IdRenderer idRenderer;
		extern ActionList actionList;
		extern Project currentProject;
		extern UISpace uiSpace;
		extern TextureManager textureManager;
		extern ModelsManager modelsManager;
		extern ShadersManager shadersManager;
		extern MaterialsManager materialsManager;
		extern PhysicsScene physicsScene;
		extern ShapesRenderer shapesRenderer;

		void update();
		void setProject(Project project);
		GameNode3D* getRoot();
		extern std::map<std::string, std::string> settings;
		void init();
		void requestModulesReload();
		void setScene(GameNode3D* node);
		void setScene(const std::string& scenePath);

		enum EngineState 
		{
			GAME_STOPPED,
			GAME_PLAYING,
			GAME_PAUSED
		};

		void play();
		void pause();
		void stop();

		void reloadModules();

		EngineState getEngineState();
	}
}