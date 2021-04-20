#pragma once
#include <vector>
#include <string>
#include <map>
#include <list>
#include <set>
#include "EventSystem/Event.h"
#include "EventSystem/EventListener.h"
using namespace std;

enum NODE_RELATION 
{
	THIS_NODE,
	NOT_RELATED_NODE,
	PARRENT_NODE,
	CHILD_NODE
};

namespace TealEngine {
	class GameNode
	{
	private:
		string entityId;
		map<EventType, EventListener> eventListeners;
		set<string> tags;

		unsigned short hierarchyDepth;
	protected:
		string name;
		list<GameNode*> childNodes;
		GameNode* parrent;

		void setParrent(GameNode* parrent);
	public:
		static void init();
		string getName();
		void rename(string name);

		//tag system
		bool hasTag(string tag);
		void addTag(string tag);
		void removeTag(string tag);
		static set<GameNode*> findNodesByTag(string tag);

		unsigned short getHierarchyDepth();
		NODE_RELATION checkRelation(GameNode* node);
		GameNode* getParrent();

		virtual GameNode* addChild(GameNode* node);
		virtual void addChilds(vector<GameNode*> nodes);
		virtual void removeChild(GameNode* node);

		template<typename T>
		std::vector<T*> findChildsByType(bool childsOfChilds = false) 
		{
			std::vector<T*> res;
			for (GameNode* node : childNodes)
			{
				if (dynamic_cast<T*>(node))
					res.push_back((T*)node);

				if (childsOfChilds)
				{
					vector<T*> childsOfChild = node->findChildsByType<T>(true);
					if (childsOfChild.size() > 0)
						res.insert(res.end(), childsOfChild.begin(), childsOfChild.end());
				}
			}
			return res;
		}

		std::vector<GameNode*> getChilds();
		std::vector<GameNode*> getAllChilds();
		std::vector<GameNode*> findChildsByName(std::string name);

		//events:
		virtual void update();
		void updateAll();
	
		void addEventListener(EventType type, eventListenerFunc ex);
		EventListener* getEventListener(EventType type);
		void handleEvent(Event* e, bool toLower = false);

		GameNode();
		virtual ~GameNode();
		static bool isNodeExist(GameNode* node);
	};
}