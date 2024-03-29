#pragma once
#include "GameNode/GameNode.h"
#include "ComponentProp.h"
#include <map>
#include <set>
#include <unordered_map>

namespace TealEngine 
{
class FrameBuffer;
	class Component 
	{
	private:
		friend void GameNode::attachComponent(Component* component);
		friend void GameNode::dettachComponent(Component* component);
		GameNode* parent;
		static std::map<int, Component*> idMap;
		static std::set<Component*> allComponents;
		static std::set<Component*> renderableComponents;
		static std::set<Component*> onKeyPressComponents;
		static std::set<Component*> onKeyReleaseComponents;
		void attachTo(GameNode* node);
		std::string factoryName;
		bool active;
		bool onAttachCallbackHasBeenCalled;
		std::string name;
		int id;
		static int lastId;
		int index;
		bool postConstructorDone;
	protected:
		std::unordered_map<std::string, IProp*> props;
	public:
		//
		bool isActive();
		//
		inline int getIndex() { return index; }
		//
		inline bool getActive() { return active; };
		//
		inline void setActive(bool active) { this->active = active; };
		//
		void rename(const std::string& name);
		//
		inline std::string getName() {return name;};
		//called every frame while in edit mode
		virtual void editorUpdate();
		//called every frame
		virtual void update();
		//called every time when parent node changes its parent
		virtual void onParentChange();
		//called just before a component update after component has been attached to any node
		virtual void onAttach();
		//called in parent node destructor before eny of its child nodes or components destroyed
		virtual void onDestroy();
		//called every time node is set to inactive after it was active
		virtual void onSleep();
		//called every time node is set to active after it was inactive
		virtual void onAwake();
		//called after 3D renderer to draw things over it
		virtual void GUIRender();
		//
		virtual void imGuiRender(const std::string& windowName);
		//
		virtual void render(ShaderProgram* shader, unsigned int stages);
		//
		virtual void renderId();
		//
		virtual void postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light, FrameBuffer* frameBuffer);
		//
		virtual void onMessageReceive();
		//
		virtual void onCollision(const Collision& collision);
		//called after some mouse button was pressed and released while mouse cusor is hovered above the node
		virtual void onClick(unsigned short button);
		//
		virtual void onWindowMousePress(unsigned short button);
		//
		virtual void onWindowMouseRelease(unsigned short button);
		//
		virtual void onMousePress(unsigned short button);
		//
		virtual void onMouseRelease(unsigned short button);
		//
		virtual void onKeyPress(unsigned int key);
		//
		virtual void onKeyRelease(unsigned int key);
		void refreshProps();
		GameNode* getParent();
		//gets parent if parent is of type T, else throws an error
		template<typename T>
		T* getParentOfType()
		{
			T* castedParent = dynamic_cast<T*>(parent);
			if (!castedParent)
				TE_DEBUG_ERROR("Parent either does not exist or isnt the right type.");
			return castedParent;
		}
		Component();
		virtual ~Component();

		//deserializes and sets a prop value
		virtual bool setProp(const Json& json);

		void storeProp(const Json& json);
		//called after setting a property with setProp or editing property in game editor 
		virtual void onPropSet(const std::string& propName);
		//removes serializable property by its name
		void removeProp(const std::string& name);

		//displays properties using ImGUI
		void displayProps(bool& componentDeleted);
		//add serializable property to the component, property object will be deleted in component desctructor
		void addProp(IProp* prop, const std::string& label);
		//
		Json toJson();
		static Component* fromJson(const Json& json);

		int getId();
		void setId(int id);
		static Component* getComponentById(int id);

		static bool isValidComponent(Component* component);

		//returns true if component has been attached to a new parent and onAttach callback has not been called yet
		bool isOnAttachCallbackHasBeenCalled();

		//cancels next onAttach callback call
		void cancelOnAttachCallback();

		void registerCallbackLists();
		static void renderAllComponents(ShaderProgram* shader, unsigned int stages);
		static void onKeyPressCallbacks(unsigned int key);
		static void onKeyReleaseCallbacks(unsigned int key);
		static Component base;
	};
}