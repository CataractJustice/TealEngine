#include <map>
#include <string>
#include "Core.h"
#include "Filesystem/loadStrFromFile.h"
#include "Graphics/Graphics.h"
#include "Graphics/Mesh/MeshUtil.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "Graphics/Renderer/DefferedRenderer.h"
#include "EventSystem/WindowEvents/WindowEvents.h"
#include "EventSystem/GameNodeEvents/GameNodeEvents.h"
#include "Graphics/Mesh/MeshRenderer.h"
#include "System/Clock.h"
#include <thread>
#include "DefaultTags.h"
#include "System/Debug.h"
#include "Physics/3D/EasyPhysics/EasyPhysicsWorld.h"
#include "libs/imgui/imgui.h"
#include "libs/imgui/backends/imgui_impl_opengl3.h"
#include "libs/imgui/backends/imgui_impl_glfw.h"


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
			GameNode3D* rootNode;
			
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

			GameNode3D* getRoot() { return rootNode; }

			void clearScene()
			{
				delete rootNode;
				rootNode = new GameNode3D();
			}

			void update()
			{
				static float updateTimer = 0.0f;
				static bool debugInfo = false;
				debugInfo = Input::Keyboard::isKeyPressed(GLFW_KEY_F1) ? true : debugInfo;
				sceneclock.update();
				updateTimer += sceneclock.deltaTime();
				//TE_DEBUG_INFO("GameNode update pass.");
				if(updateTimer >= 1.0f / 60.0f) 
				{
					updateTimer = 0.0f;
					Scene::rootNode->updateAll();
					GameNode::cleanUp();
					Physics::EasyPhysics::EasyPhysicsWorld::instance.solveCollisions();
					//TE_DEBUG_INFO("Render pass.");
					Scene::renderer.render(Scene::rootNode);

					//TE_DEBUG_INFO("Rendering to main buffer.");
					FrameBuffer::unbind();
					if (Input::Keyboard::isKeyPressed(GLFW_KEY_0))
						Render::renderTexture(Input::Mouse::getScrollPos());
					else
						Render::renderTexture(Scene::renderer.getActiveCamera()->renderTexture.id());

					//TE_DEBUG_INFO("GUI rendering.");
					rootNode->GUIrender();

					ImGui_ImplOpenGL3_NewFrame();
					ImGui_ImplGlfw_NewFrame();
					ImGui::NewFrame();
					rootNode->imGUIrender();
					if (debugInfo) 
					{
						ImGui::Begin("Game Explorer", &debugInfo);
						if (ImGui::TreeNode("Scene tree")) 
						{
							rootNode->displayNodeTree(false);
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("Textures"))
						{
							static int tid;
							ImGui::InputInt("Texture id", &tid, 1, 10);
							ImGui::Image(ImTextureID((long)tid), ImVec2(128.0f, 128.0f));
							ImGui::TreePop();
						}
						ImGui::End();
					}
					ImGui::Render();
					ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

					Graphics::display();
				}
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
			Scene::rootNode = new GameNode3D();

			TE_DEBUG_INFO("Init Dear ImGUI");
			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			// Setup Platform/Renderer bindings
			ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)Graphics::window->gl_window_ptr_(), true);
			ImGui_ImplOpenGL3_Init("#version 130");
			// Setup Dear ImGui style
			ImGui::StyleColorsLight();
		}

		bool isServer() 
		{
			return false;
		}
	}
}