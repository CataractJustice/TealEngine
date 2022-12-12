#pragma once
#include "GameNode.h"

#define RegisterComponent()

namespace TealEngine 
{
	class Component 
	{
	private:
		friend void GameNode::attachComponent(Component* component);
		friend void GameNode::dettachComponent(Component* component);
		GameNode* parrent;
		static std::vector<Component*> components;
		void attachTo(GameNode* node);
		std::string factoryName;
		bool active;
	public:
		bool isActive();
		inline bool getActive() { return active; };
		inline void setActive(bool active) { this->active = active; };

		//called every frame
		virtual void update();
		//called every time when parrent node changes its parrent
		virtual void onParrentChange();
		//called every time when component attached to any node
		virtual void onAttach();
		//called in parrent node destructor before eny of its child nodes or components destroyed
		virtual void onDestroy();
		//called every time node is set to inactive after it was active
		virtual void onSleep();
		//called every time node is set to active after it was inactive
		virtual void onAwake();
		//called after 3D renderer to draw things over it
		virtual void GUIrender();
		//
		virtual void imGUIrender();
		//
		virtual void render(ShaderProgram* shader, unsigned int stages);
		//
		virtual void postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light);
		//
		virtual void onMessageReceive();
		//
		virtual void onCollision(const Physics::Collision& collision);
		GameNode* getParrent();
		//gets parrent if parrent is of type T, else throws an error
		template<typename T>
		T* getParrentOfType()
		{
			T* castedParrent = dynamic_cast<T*>(parrent);
			if (!castedParrent)
				TE_DEBUG_ERROR("Parrent either does not exist or isnt the right type.");
			return castedParrent;
		}
		Component();
		virtual ~Component();
	};
}