#pragma once
#include "GameNode/Component.h"
#include "System/Clock.h"
#include "libs/glm/glm.hpp"
namespace TealEngine {
	class FreeCam : public Component
	{
	private:
		Clock clock;
		EventListener mouseMoveListener;
	public:
		float sensitivity;
		float gravity;
		glm::vec2 viewdir;

		FreeCam();
		void update();
		void onMouseMove(Event* e);
	};
}