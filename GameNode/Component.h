#pragma once
#include "GameNode.h"

namespace TealEngine 
{
	class Component 
	{
	private:
		friend void GameNode::attachComponent(Component* component);
		friend void GameNode::dettachComponent(Component* component);
		GameNode* parrent;
		void attachTo(GameNode* node);
	public:
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
		GameNode* getParrent();
		~Component();
	};
}