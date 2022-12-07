#pragma once
#include "Core.h"
#include "Scene.h"

namespace TealEngine 
{
	inline void Run() {
		Core::init();
		setupScene(Core::Scene::getRoot());
		while (!Graphics::window->shouldClose())
		{
			Core::Scene::update();
		}
	}
}