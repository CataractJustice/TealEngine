#pragma once
#include <map>
#include <string>
#include <chrono>
#include "System/Input.h"
#include "GameNode/GameNode3D.h"
#include "Editor/ActionList.h"
#include "Editor/UI/UISpace.h"
#include "Project.h"
#include "ResourceManagment/TextureManager.h"
#include "ResourceManagment/ModelsManager.h"
#include "ResourceManagment/ShadersManager.h"
#include "ResourceManagment/MaterialsManager.h"
#include "Physics/PhysicsScene.h"
#include "ResourceManagment/FontManager.h"
#include "System/Profiler.h"
#include "Graphics/Mesh/MeshRenderer.h"
#include "Kirino/UniformBufferObject.h"
#include "Graphics/ShapesRenderer.h"
#include "Network/Connection.h"
#include "Network/NetworkDictionary.h"
#include "System/Arguments.h"
namespace TealEngine 
{
	class SceneNode;
	namespace Core 
	{
		extern "C" ActionList actionList;
		extern "C" Project currentProject;
		extern "C" UISpace uiSpace;
		extern "C" TextureManager textureManager;
		extern "C" ModelsManager modelsManager;
		extern "C" ShadersManager shadersManager;
		extern "C" MaterialsManager materialsManager;
		extern "C" PhysicsScene* physicsScene;
		extern "C" FontManager fontManager;
		extern "C" Profiler profiler;
		extern "C" Kirino::UniformBufferObject cameraUBO;
		extern "C" Kirino::UniformBufferObject transformUBO;
		extern "C" const Camera* camera;
		extern "C" ShapesRenderer shapesRenderer;
		extern "C" Client client;
		extern "C" Host host;
		extern "C" NetworkDictionary networkDictionary;
		extern "C" LaunchArguments launchArguments;
 		void update(); 

		void setProject(Project project);

		GameNode* getRoot();
		
		extern std::map<std::string, std::string> settings;
		void init(int argc, char** argv);
		void close();
		void requestModulesReload();
		void setScene(GameNode* node);
		void setScene(const std::string& scenePath);
		void setCamera(const Camera* camera);

		SceneNode* sceneNode();
		//void hostServer();
		//oid connectToServer();

		enum EngineState 
		{
			GAME_STOPPED,
			GAME_PLAYING,
			GAME_PAUSED
		};

		void play();
		void pause();
		void stop();

		unsigned int getFrameId();

		void reloadModules();

		EngineState getEngineState();
	}
}