#include <map>
#include <string>
#include "Core.h"
#include <fstream>
#include "FileSystem/loadStrFromFile.h"
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
#include "NlohmannJson/json.hpp"
#include "GameNode/ComponentLoader.h"

using Json = nlohmann::json;

namespace TealEngine
{
	namespace Core
	{
		namespace Scene
		{
			Clock sceneclock;
			EventListener rendererResizeEvent;
			DefferedRenderer renderer;
			GameNode3D* rootNode;
			ComponentLoader componentLoader;
			
			
			void addNode(GameNode* node)
			{
				rootNode->addChild(node);
			}

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
						if(Scene::renderer.getActiveCamera()) Render::renderTexture(Scene::renderer.getActiveCamera()->renderTexture.id());

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

			GameNode3D* nodeFromJson(Json& json) 
			{
				GameNode3D* node = new GameNode3D();
				Json& components = json["components"];

				if(json.find("components") != json.cend()) 
				{
					for(Json& componentJson : components)
					{
						std::string componentClass = componentJson["class"];
						node->attachComponent(componentLoader.createComponent(componentClass));
					}
				}

				if(json.find("nodes") != json.cend()) 
				{
					Json& subNodes = json["nodes"];
					for(Json& subNodeJson : subNodes) 
					{
						GameNode3D* subNode = nodeFromJson(subNodeJson);
						node->addChild(subNode);
					}
				}

				if(json.find("name") != json.cend()) 
				{
					node->rename(json["name"]);
				}
				
				return node;
			}

			void loadFromJson(std::string path) 
			{
				std::ifstream sceneFile(path);
				Json sceneJson = Json::parse(sceneFile);
				rootNode = nodeFromJson(sceneJson);
			}
		}

		void init()
		{
			TE_DEBUG_INFO("loading settings file.");
			
			std::fstream settingsFile("settings.json");
			Json settingsJson = Json::parse(settingsFile);

			TE_DEBUG_INFO("Graphics initialization.");
			Graphics::init(settingsJson["title"]);
			TE_DEBUG_INFO("Loading resources.");
			Resources::load(settingsJson["resources"]);
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
					if(Scene::renderer.getActiveCamera()) 
					{
						Scene::renderer.getActiveCamera()->setPerspectiveProjection(Scene::renderer.getActiveCamera()->getFOV(), float(width) / float(height), Scene::renderer.getActiveCamera()->getNear(), Scene::renderer.getActiveCamera()->getFar());
						Scene::renderer.getActiveCamera()->renderTexture.create(width, height);
					}
				});
			Graphics::window->WindowResize.subscribe(&Scene::rendererResizeEvent);

			Scene::componentLoader.loadDir("mods");
			Scene::loadFromJson(settingsJson["scene"]);

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
			
			TE_DEBUG_INFO("Init finished");
		}

		bool isServer() 
		{
			return false;
		}
	}
}