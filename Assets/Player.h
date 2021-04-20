#pragma once
#include "../GameNode/Entity/Entity.h"
#include "../System/Input.h"
#include "../System/Clock.h"
#include "../EventSystem/MouseEvents/MouseEvents.h"
#include "../EventSystem/NetworkEvents/MsgReciveEvent.h"
namespace TealEngine {
	class Player : public Entity
	{
	private:
		Clock clock;
		EventListener mouseMoveListener;
	public:
		float sensivity;
		float gravity;
		vec2 viewdir;

		Player() 
		{
			this->sensivity = 1.0f;
			this->mouseMoveListener = eventListenerBind(&Player::onMouseMove, this);
			Input::Mouse::MouseMove.subscribe(&mouseMoveListener);
			
		}

		void update() 
		{
			vec3 front = cross(this->transform.right(), vec3(0.0f, 1.0f, 0.0f));
			clock.update();
			if (Input::Keyboard::isKeyPressed(GLFW_KEY_W))
			{
				this->transform.translate(this->transform.forward() * 1.0f);
			}
			if (Input::Keyboard::isKeyPressed(GLFW_KEY_A))
			{
				this->transform.translate(this->transform.right() * 1.0f);
			}
			if (Input::Keyboard::isKeyPressed(GLFW_KEY_S))
			{
				this->transform.translate(-this->transform.forward() * 1.0f);
			}
			if (Input::Keyboard::isKeyPressed(GLFW_KEY_D))
			{
				this->transform.translate(-this->transform.right() * 1.0f);
			}
			if (Input::Keyboard::isKeyPressed(GLFW_KEY_SPACE))
			{
				this->transform.translate(this->transform.up() * 1.0f);
			}
			if (Input::Keyboard::isKeyPressed(GLFW_KEY_LEFT_SHIFT))
			{
				this->transform.translate(-this->transform.up() * 1.0f);
			}

			;
		}

		void onMouseMove(Event* e) 
		{
			MouseMoveEvent mme = *((MouseMoveEvent*)(e));
			viewdir.x -= mme.xdelta * sensivity;
			viewdir.y += mme.ydelta * sensivity;
			viewdir.y = glm::clamp(viewdir.y, -90.0f, 90.0f);

			this->transform.setRotation(0.0, vec3(1.0));
			this->transform.rotate(viewdir.x, vec3(0.0, 1.0, 0.0));
			this->transform.rotate(viewdir.y, vec3(1.0, 0.0, 0.0));
		}
	};


	class ServerSidePlayer : public Entity
	{
	private:
		void messageDispatcher(Event* e)
		{
			if (e->getType() == MSG_RECIVED)
			{
				MsgReciveEvent* mre = (MsgReciveEvent*)e;
				this->transform.setPosition(vec3(mre->message.getFieldValue<float>(2), mre->message.getFieldValue<float>(3), mre->message.getFieldValue<float>(4)));
			}
		}
	public:
		ServerSidePlayer()
		{
			this->addEventListener(MSG_RECIVED, eventListenerBind(&ServerSidePlayer::messageDispatcher, this));
		}
	};
}