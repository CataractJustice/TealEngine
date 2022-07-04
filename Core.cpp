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


namespace TealEngine
{
	namespace Core
	{
		
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
					
					renderer.pop(node);

					if (node->hasTag(GUI_ELEMENT_TAG))
						GUIBody->removeChild(node);
				}
			}

			void clearScene()
			{
				delete rootNode;
				rootNode = new GameNode();
			}

			void update()
			{
				//TE_DEBUG_INFO("GameNode update pass.");
				Scene::rootNode->updateAll();
				sceneclock.update();

				//TE_DEBUG_INFO("Render pass.");
				Scene::renderer.render();

				//TE_DEBUG_INFO("Rendering to main buffer.");
				FrameBuffer::unbind();
				if (Input::Keyboard::isKeyPressed(GLFW_KEY_0))
					Render::renderTexture(Input::Mouse::getScrollPos());
				else
					Render::renderTexture(Scene::renderer.getActiveCamera()->renderTexture.id());

				//TE_DEBUG_INFO("GUI rendering.");
				CoreGUIRenderer.render();

				Graphics::display();
				

			}

			float deltaTime() 
			{
				return sceneclock.deltaTime();
			}
		}

		float quadratic(float x) {
			return x * x;
		}

		void init()
		{
			TE_DEBUG_INFO("loading settings file.");
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
				string second = settingsStr.substr(size_t(fpos + 1), size_t(epos - fpos - 1));
				settings[first] = second;
				settingsStr = settingsStr.substr(size_t(epos + 1), size_t(settingsStr.length() - epos - 1));
			}
			TE_DEBUG_INFO("Graphics initialization.");
			Graphics::init(settings["title"]);
			TE_DEBUG_INFO("Loading resources.");
			Resources::load(settings["resource_pack"]);
			TE_DEBUG_INFO("Generating models.");
			BasicMeshes::init();
			TE_DEBUG_INFO("Initializing input.");
			Input::init();
			TE_DEBUG_INFO("Initializing light.");
			lightInit();

			TE_DEBUG_INFO("Initializing renderer.");
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
			Scene::rootNode = new GameNode();
			Scene::rootNode->addEventListener(CHILD_ADDED, eventListenerFunc(&Scene::onChildNodeAdd));
			Scene::rootNode->addEventListener(CHILD_REMOVED, eventListenerFunc(&Scene::onChildNodeRemove));

			Scene::GUIBody = new GUIElement();
			Scene::CoreGUIRenderer.push(Scene::GUIBody);
		}

		bool isServer() 
		{
			return false;
		}
	}
}