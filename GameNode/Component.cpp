#include "Component.h"
#include "libs/imgui/imgui.h"
namespace TealEngine 
{
	void Component::attachTo(GameNode* node) 
	{
		if (parrent != node) 
		{
			if (parrent)
				this->parrent->dettachComponent(this);
			this->parrent = node;
			onAttach();
		}
	}

	bool Component::isActive() 
	{
		return this->getParrent()->getActive();
	}

	void Component::update() {};
	void Component::onAwake() {};
	void Component::onDestroy() {};
	void Component::onParrentChange() {};
	void Component::onSleep() {};
	void Component::onAttach() {};
	void Component::onMessageReceive() {};
	void Component::onCollision(const Physics::Collision& collision) {};
	void Component::GUIrender() {};
	void Component::imGUIrender() {};
	void Component::render(ShaderProgram* shader, unsigned int stages) {};
	void Component::postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light) {};

	GameNode* Component::getParrent() { return parrent; }

	Component::Component() : active(true), parrent(nullptr) {};

	Component::~Component() 
	{
		if (parrent)
			parrent->dettachComponent(this);
	}
}