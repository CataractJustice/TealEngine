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
		//initializing
		//
		//loading scene
		//	loading resources
		//	loading objects
		//
		//updating scene
		//	window events
		//	render 
		//	update | fixed update
		//		physic engine
		//			check collisions
		//			create collision evens
		//			apply forces
		//loop

		//scene
		//	GameNodes
		//	Renderer	
		namespace Scene 
		{
			extern DefferedRenderer renderer;
			void addNode(GameNode* node);
			void clearScene();
			void update();
			float deltaTime();
			GameNode3D* getRoot();
		}

		extern std::map<std::string, std::string> settings;
		void init();
		bool isServer();
	}
}