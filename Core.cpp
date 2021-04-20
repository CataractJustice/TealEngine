#pragma once
#include <map>
#include <string>
#include "Core.h"
#include "Filesystem/loadStrFromFile.h"
#include "Graphics/Graphics.h"
#include "Graphics/Mesh/MeshUtil.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "Graphics/Renderer/DefferedRenderer.h"
#include "EventSystem/WindowEvents/WindowEvents.h"
#include "Physics/RigidBody.h"
#include "EventSystem/GameNodeEvents/GameNodeEvents.h"
#include "Graphics/Mesh/MeshRenderer.h"
#include "Network/Server.h"
#include "Network/ClientNode.h"
#include <btBulletDynamicsCommon.h>
#include <thread>
namespace TealEngine
{
	namespace Core
	{
		std::map<std::string, std::string> settings;
		ClientNode* clientNode;
		Server* server;
		namespace Scene
		{
			EventListener rendererResizeEvent;
			DefferedRenderer renderer;
			btDynamicsWorld* btEngine;
			GameNode* rootNode;
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


			void onChildNodeAdd(Event* e)
			{
				if (e->getType() == CHILD_ADDED)
				{
					std::vector<GameNode*> childs = ((ChildNodeAddEvent*)e)->node->getAllChilds();
					for (GameNode* child : childs)
					{
						RigidBody* body = dynamic_cast<RigidBody*>(child);
						if(body)
							btEngine->addRigidBody(body->getBody());
					}
					renderer.push(((ChildNodeAddEvent*)e)->node);

					RigidBody* body = dynamic_cast<RigidBody*>(((ChildNodeAddEvent*)e)->node);
					if (body) 
					{
						btEngine->addRigidBody(body->getBody());
					}
				}
			}

			void onChildNodeRemove(Event* e)
			{
				if (e->getType() == CHILD_REMOVED)
				{
					GameNode* node = ((ChildNodeRemoveEvent*)e)->node;
					if(node->hasTag("MeshRenderer"))
						renderer.pop((MeshRenderer*)node);

					RigidBody* body = dynamic_cast<RigidBody*>(node);
					if (body)
						btEngine->removeRigidBody(body->getBody());
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
				Render::rednerTexture(Input::Mouse::getScrollPos());
				Graphics::display();
				Scene::rootNode->updateAll();
				Scene::btEngine->stepSimulation(0.01f);
			}
		}

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

		void init()
		{
			std::string settingsStr = loadStrFromFile("settings.txt");
			int fpos = 0;
			int epos = 0;
			while (epos != std::string::npos)
			{
				fpos = settingsStr.find(":");
				epos = settingsStr.find("\n");
				string first = settingsStr.substr(0, fpos);
				string second = settingsStr.substr(fpos + 1, epos - fpos - 1);
				settings[first] = second;
				settingsStr = settingsStr.substr(epos + 1, settingsStr.length() - epos - 1);
			}

			Graphics::glInit(settings["title"]);
			Resources::load(settings["resource_pack"]);
			BasicMeshes::init();
			Input::init();
			lightInit();
			PrefabFactory::init();

			Scene::renderer.resize(Graphics::Window::getWindowWidth(), Graphics::Window::getWindowHeight());
			Scene::renderer.setDepthTest(true);
			Scene::renderer.setDepthClear(true);
			Scene::renderer.setColorClear(true);
			Scene::renderer.setClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
			Scene::rendererResizeEvent = std::function<void(Event*)>([] (Event* e) 
				{		
					unsigned int width = ((WindowResizeEvent*)(e))->width;
					unsigned int height = ((WindowResizeEvent*)(e))->height;

					Scene::renderer.resize(width, height);
					Scene::renderer.getActiveCamera()->setPerspectiveProjection(90.0f, float(width) / float(height), 0.1, 5000.0f);
					Scene::renderer.getActiveCamera()->renderTexture.create(width, height);
				});
			Graphics::Window::WindowResize.subscribe(&Scene::rendererResizeEvent);
			
			btBroadphaseInterface* broadphase = new btDbvtBroadphase();
			btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
			btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
			btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
			Scene::btEngine = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
			Scene::btEngine->getSolverInfo().m_numIterations = 4;
			Scene::btEngine->setGravity(btVector3(0.0f, -9.8f, 0.0f));

			Scene::rootNode = new GameNode();
			Scene::rootNode->addEventListener(CHILD_ADDED, eventListenerFunc(&Scene::onChildNodeAdd));
			Scene::rootNode->addEventListener(CHILD_REMOVED, eventListenerFunc(&Scene::onChildNodeRemove));

			enet_initialize();
			thread* serverThread = new thread(startServer);

			ClientNode* client = new ClientNode("127.0.0.1", 8888);
			Scene::addNode(client);
		}
	}
}