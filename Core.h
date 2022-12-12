#pragma once
#include <map>
#include <string>
#include "Resources.h"
#include "System/Input.h"
#include "Graphics/Renderer/DefferedRenderer.h"
#include "GameNode/GameNode3D.h"
namespace TealEngine 
{
	namespace Core 
	{
		namespace Scene 
		{
			extern DefferedRenderer renderer;
			void addNode(GameNode* node);
			void clearScene();
			void update();
			float deltaTime();
			GameNode3D* getRoot();
			void loadFromJson();
		}

		extern std::map<std::string, std::string> settings;
		void init();
		bool isServer();
	}
}