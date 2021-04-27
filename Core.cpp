#pragma once
#include <map>
#include <string>
#include "Core.h"
#include "Filesystem/loadStrFromFile.h"
#include "Graphics/Graphics.h"
#include "Graphics/Mesh/MeshUtil.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "Graphics/Renderer/DefferedRenderer.h"
#include "Graphics/GUI/GUIRenderer.h"
#include "EventSystem/WindowEvents/WindowEvents.h"
#include "EventSystem/GameNodeEvents/GameNodeEvents.h"
#include "Graphics/Mesh/MeshRenderer.h"
#include "System/Clock.h"
#include <thread>
#include "DefaultTags.h"
#include "System/Debug.h"
//#define TE_MULTIPLAYER
#ifdef TE_MULTIPLAYER
#include "enet/enet.h"
#include "Network/Server.h"
#include "Network/ClientNode.h"
#endif

#ifdef BULLET_PHYSICS
#include <btBulletDynamicsCommon.h>
#include "Physics/RigidBody.h"
#endif


namespace TealEngine
{
	namespace Core
	{
#ifdef TE_MULTIPLAYER
		ClientNode* clientNode;
		Server* server;
#endif
		
		std::map<std::string, std::string> settings;
		namespace Scene
		{
			Clock sceneclock;
			EventListener rendererResizeEvent;
			DefferedRenderer renderer;
			GUIRenderer CoreGUIRenderer;
			GameNode* rootNode;
			GUIElement* GUIBody;
			
#ifdef BULLET_PHYSICS
			btDynamicsWorld* btEngine;
			void addNode(GameNode* node)
			{
				rootNode->addChild(node);
				std::vector<RigidBody*> bodies = node->findChildsByType<RigidBody>(true);
				for (RigidBody* body : bodies)
				{
					btRigidBody* btBody = body->getBody();
					btEngine->addRigidBody(btBody);
				}
			}
#else
			void addNode(GameNode* node)
			{
				rootNode->addChild(node);
			}
#endif

			void onChildNodeAdd(Event* e)
			{
				if (e->getType() == CHILD_ADDED)
				{
					GameNode* node = ((ChildNodeAddEvent*)e)->node;

					std::vector<GameNode*> childs = ((ChildNodeAddEvent*)e)->node->getAllChilds();

#ifdef BULLET_PHYSICS
					for (GameNode* child : childs)
					{
						RigidBody* body = dynamic_cast<RigidBody*>(child);
						if (body)
							btEngine->addRigidBody(body->getBody());
					}

					RigidBody* body = dynamic_cast<RigidBody*>(((ChildNodeAddEvent*)e)->node);
					if (body)
					{
						btEngine->addRigidBody(body->getBody());
					}
#endif // BULLET_PHYSICS

					renderer.push(node);

					if (node->hasTag(GUI_ELEMENT_TAG))
						GUIBody->removeChild(node);

					
				}
			}

			void onChildNodeRemove(Event* e)
			{
				if (e->getType() == CHILD_REMOVED)
				{
					GameNode* node = ((ChildNodeRemoveEvent*)e)->node;
					if(node->hasTag("MeshRenderer"))
						renderer.pop((MeshRenderer*)node);

					if (node->hasTag(GUI_ELEMENT_TAG))
						GUIBody->removeChild(node);
#ifdef BULLET_PHYSICS
					RigidBody* body = dynamic_cast<RigidBody*>(node);
					if (body)
						btEngine->removeRigidBody(body->getBody());
#endif // BULLET_PHYSICS
				}
			}

			void clearScene()
			{
				delete rootNode;
				rootNode = new GameNode();
			}

			void update()
			{
				Scene::renderer.render();
				FrameBuffer::unbind();
				if (Input::Keyboard::isKeyPressed(GLFW_KEY_0))
					Render::rednerTexture(Input::Mouse::getScrollPos());
				else
					Render::rednerTexture(Scene::renderer.getActiveCamera()->renderTexture.id());
				CoreGUIRenderer.render();
				Graphics::display();
				Scene::rootNode->updateAll();
				sceneclock.update();
#ifdef BULLET_PHYSICS
				Scene::btEngine->stepSimulation(0.01f);
#endif // BULLET_PHYSICS

			}

			float deltaTime() 
			{
				return sceneclock.deltaTime();
			}
		}
#ifdef TE_MULTIPLAYER
		void startServer()
		{
			server = new Server();
			server->start(8888, 20);
			while (true)//?
			{
				try
				{
					server->update();
				}
				catch (char* err)
				{
					std::cout << err;
				}
			}
		}

		void connectToServer()
		{

		}
#endif // TE_MULTIPLAYER

		

		void init()
		{
			int settingsLoadErrorCode = 0;
			std::string settingsStr = loadStrFromFile("settings.txt", &settingsLoadErrorCode);
			if (settingsLoadErrorCode)
			{
				TE_DEBUG_ERROR("Could not find settings.txt file");
			}
			int fpos = 0;
			int epos = 0;
			settings["resource_pack"] = "res";
			while (epos != std::string::npos)
			{
				fpos = settingsStr.find(":");
				epos = settingsStr.find("\n");
				string first = settingsStr.substr(0, fpos);
				string second = settingsStr.substr(fpos + 1, epos - fpos - 1);
				settings[first] = second;
				settingsStr = settingsStr.substr(epos + 1, settingsStr.length() - epos - 1);
			}

			Graphics::init(settings["title"]);
			Resources::load(settings["resource_pack"]);
			BasicMeshes::init();
			Input::init();
			lightInit();

			Scene::renderer.resize(Graphics::window->getWindowWidth(), Graphics::window->getWindowHeight());
			Scene::renderer.setDepthTest(true);
			Scene::renderer.setDepthClear(true);
			Scene::renderer.setColorClear(true);
			Scene::renderer.setClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
			Scene::rendererResizeEvent = std::function<void(Event*)>([] (Event* e) 
				{		
					unsigned int width = ((WindowResizeEvent*)(e))->width;
					unsigned int height = ((WindowResizeEvent*)(e))->height;

					Scene::renderer.resize(width, height);
					Scene::renderer.getActiveCamera()->setPerspectiveProjection(Scene::renderer.getActiveCamera()->getFOV(), float(width) / float(height), Scene::renderer.getActiveCamera()->getNear(), Scene::renderer.getActiveCamera()->getFar());
					Scene::renderer.getActiveCamera()->renderTexture.create(width, height);
				});
			Graphics::window->WindowResize.subscribe(&Scene::rendererResizeEvent);
			
#ifdef BULLET_PHYSICS
			btBroadphaseInterface* broadphase = new btDbvtBroadphase();
			btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
			btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
			btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
			Scene::btEngine = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
			Scene::btEngine->getSolverInfo().m_numIterations = 4;
			Scene::btEngine->setGravity(btVector3(0.0f, -9.8f, 0.0f));
#endif
			Scene::rootNode = new GameNode();
			Scene::rootNode->addEventListener(CHILD_ADDED, eventListenerFunc(&Scene::onChildNodeAdd));
			Scene::rootNode->addEventListener(CHILD_REMOVED, eventListenerFunc(&Scene::onChildNodeRemove));

			Scene::GUIBody = new GUIElement();
			Scene::CoreGUIRenderer.push(Scene::GUIBody);

#ifdef  TE_MULTIPLAYER
			enet_initialize();
			//thread* serverThread = new thread(startServer);

			//ClientNode* client = new ClientNode("127.0.0.1", 8888);
			//Scene::addNode(client);
#endif //  TE_MULTIPLAYER
		}
	}
}