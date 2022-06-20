#include "FreeCam.h"
#include "EventSystem/MouseEvents/MouseEvents.h"
#include "EventSystem/NetworkEvents/MsgReciveEvent.h"
#include "GameNode/GameNode3D.h"
#include "System/Input.h"

namespace TealEngine {
		FreeCam::FreeCam()
		{
			this->sensitivity = 1.0f;
			this->mouseMoveListener = eventListenerBind(&FreeCam::onMouseMove, this);
			Input::Mouse::MouseMove.subscribe(&mouseMoveListener);

		}

		void FreeCam::update()
		{
			Transform& transform = getParrentOfType<GameNode3D>()->getRelativeTransform();
			glm::vec3 front = glm::cross(transform.right(), glm::vec3(0.0f, 1.0f, 0.0f));
			clock.update();
			if (Input::Keyboard::isKeyPressed(GLFW_KEY_W))
			{
				transform.translate(transform.forward() *clock.deltaTime());
			}
			if (Input::Keyboard::isKeyPressed(GLFW_KEY_A))
			{
				transform.translate(transform.right() * clock.deltaTime());
			}
			if (Input::Keyboard::isKeyPressed(GLFW_KEY_S))
			{
				transform.translate(-transform.forward() * clock.deltaTime());
			}
			if (Input::Keyboard::isKeyPressed(GLFW_KEY_D))
			{
				transform.translate(-transform.right() * clock.deltaTime());
			}
			if (Input::Keyboard::isKeyPressed(GLFW_KEY_SPACE))
			{
				transform.translate(transform.up() * clock.deltaTime());
			}
			if (Input::Keyboard::isKeyPressed(GLFW_KEY_LEFT_SHIFT))
			{
				transform.translate(-transform.up() * clock.deltaTime());
			}
		}

		void FreeCam::onMouseMove(Event* e)
		{
			Transform& transform = getParrentOfType<GameNode3D>()->getRelativeTransform();
			MouseMoveEvent mme = *((MouseMoveEvent*)(e));
			viewdir.x -= mme.xdelta * sensitivity;
			viewdir.y += mme.ydelta * sensitivity;
			viewdir.y = glm::clamp(viewdir.y, -90.0f, 90.0f);

			transform.setRotation(0.0, glm::vec3(1.0));
			transform.rotate(viewdir.x, glm::vec3(0.0, 1.0, 0.0));
			transform.rotate(viewdir.y, glm::vec3(1.0, 0.0, 0.0));
		}
}