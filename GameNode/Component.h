#pragma once
#include "GameNode/GameNode.h"
#include "ComponentProp.h"
#include <map>
#include <set>
#include <unordered_map>
#include "Network/Connection.h"
#include "Kirino/StateColletion.h"
namespace TealEngine 
{
	struct FrustumPlanes 
	{
		glm::vec3 nearNormal;
		float nearD;
		glm::vec3 farNormal;
		float farD;
		glm::vec3 leftNormal;
		float leftD;
		glm::vec3 rightNormal;
		float rightD;
		glm::vec3 upNormal;
		float upD;
		glm::vec3 bottomNormal;
		float bottomD;
	};
	struct RenderTexturesCollection 
	{
		unsigned int color, position, normal, specular, unlit, depth, transparentPositionAlpha, transparentNormal;
	};
	class FrameBuffer;
	class Component 
	{
	private:
		friend void GameNode::attachComponent(Component* component);
		friend void GameNode::dettachComponent(Component* component);
		GameNode* parent;
		static std::map<int, Component*> idMap;
		static std::set<Component*> allComponents;
		void attachTo(GameNode* node);
		std::string factoryName;
		bool active;
		bool onAttachCallbackHasBeenCalled;
		std::string name;
		int id;
		static int lastId;
		int index;
		bool postConstructorDone;
		NetworkId netId;
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
		virtual void postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light, FrameBuffer* frameBuffer);
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
		virtual void onKeyRepeat(unsigned int key);
		//
		virtual void onKeyRelease(unsigned int key);
		//
		virtual void onChar(unsigned int unicode);
		//called every time a child added to this component parent node or one of its child nodes
		virtual void onChildAdd(GameNode* node, GameNode* parent);
		//called every time a child removed from this component parent node or one of its child nodes
		virtual void onChildRemove(GameNode* node, GameNode* parent);
		//called every time a child added to this component parent node or one of its child nodes
		virtual void onComponentAdd(Component* component, GameNode* parent);
		//called every time a component removed from this component parent node or one of its child nodes
		virtual void onComponentRemove(Component* component, GameNode* parent);
		//called every time a node added to the scene
		virtual void onSceneChildAdd(GameNode* node, GameNode* parent);
		//called every time a node removed from the scene
		virtual void onSceneChildRemove(GameNode* node, GameNode* parent);
		//called every time a component added to the scene
		virtual void onSceneComponentAdd(Component* component, GameNode* parent);
		//called every time a component removed from the scene
		virtual void onSceneComponentRemove(Component* component, GameNode* parent);
		//
		virtual void onWindowResize(int width, int height);
		//
		virtual void onPropRefresh();
		//
		virtual void render(Kirino::StateColletion& state, const FrustumPlanes& frustum, const RenderTexturesCollection& renderTextures);
		//
		virtual void shadowRender(Kirino::StateColletion& state, const FrustumPlanes& frustum, const RenderTexturesCollection& renderTextures);
		//
		virtual void postRender(Kirino::StateColletion& state, const FrustumPlanes& frustum, const RenderTexturesCollection& renderTextures);
		//
		virtual void transparentRender(Kirino::StateColletion& state, const FrustumPlanes& frustum, const RenderTexturesCollection& renderTextures);
		/*
		Network section
		*/
		//called on a host every time a new client is connected
		virtual void onHostPeerConnect(PeerInfo& peerInfo);
		//called on a host every time a client disconnects
		virtual void onHostPeerDisconnect(PeerInfo& peerInfo);
		//called on a host every time a packet is received from a client
		virtual void onHostPeerPacket(PeerInfo& peerInfo, const void* data, int size);
		//called on a client after connecting to a host
		virtual void onClientConnect();
		//called on a client after disconnecting from a host
		virtual void onClientDisconnect();
		//called on a client after receiving a packet from a host
		virtual void onClientPacket(const void* data, int size);
		//called if client fails to connect to a host
		virtual void onClientConnectFail();

		void setNetworkId(NetworkId netId);
		inline NetworkId getNetworkId() const { return netId; }

		void refreshProps();
		GameNode* getParent();
		const GameNode* getParentConst() const;
		//gets parent if parent is of type T, else throws an error
		template<typename T>
		T* getParentOfType()
		{
			T* castedParent = dynamic_cast<T*>(parent);
			return castedParent;
		}
		//
		template<typename T>
		const T* getParentOfTypeConst() const
		{
			T* castedParent = dynamic_cast<T*>(parent);
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

		static Component base;
	};
}