#include <map>
#include <string>
#include "Core.h"
#include <fstream>
#include "FileSystem/loadStrFromFile.h"
#include "Graphics/Graphics.h"
#include "Graphics/Mesh/MeshUtil.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "Graphics/Renderer/DeferredRenderer.h"
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
#include "System/DLLoader.h"
#include "GameNode/ComponentFactory.h"
#include "Editor/UI/UISpace.h"
#include "Editor/UI/EditorUI/TreeDisplay.h"
#include "Editor/UI/EditorUI/CameraViewport.h"
#include "Editor/UI/EditorUI/GameNodePropsWindow.h"
#include "Graphics/Renderer/IdRenderer.h"
#include "Editor/UI/SelectedObjectOutlinePostProcess.h"
#include "Editor/UI/EditorUI/ComponentsExplorer.h"
#include "Editor/UI/EditorUI/GameAssetsBrowser.h"
#include "Editor/UI/EditorUI/ProjectPropertiesWindow.h"

using Json = nlohmann::json;

namespace TealEngine
{
	namespace Core
	{
		DeferredRenderer renderer;
		IdRenderer idRenderer;
		GameNode3D* sceneRoot;
		Clock sceneclock;
		UISpace uiSpace;
		GameNode3D* idCameraNode;
		Camera* idCamera;
		ActionList actionList;
		Project currentProject;

		Json originalSceneJson;

		GameAssetsBrowser* gameAssetsBrowser;
		bool modulesNeedReload = false;
		void requestModulesReload() { modulesNeedReload = true; }

		GameNode3D* nextScene = nullptr;

		enum EngineState 
		{
			GAME_STOPPED,
			GAME_PLAYING,
			GAME_PAUSED
		};

		EngineState engineState = EngineState::GAME_STOPPED;

		void update()
		{
			//capping update rate to a 60 fps here
			//to-do:
			//-add unfixed update with delta time passed as an argument
			//-add a way to customize update rate
			//-maybe add a way to make multiple update rates?
			static float updateTimer = 0.0f;
			sceneclock.update();
			updateTimer += sceneclock.deltaTime();
			if(updateTimer >= 1.0f / 60.0f) 
			{
				if(modulesNeedReload) 
				{
					currentProject.buildLibs();
					
					if(sceneRoot && gameAssetsBrowser && gameAssetsBrowser->getParent() == sceneRoot)
						sceneRoot->dettachComponent(gameAssetsBrowser);
						
					Json currentScene = sceneRoot->toJson();
					if(sceneRoot) delete sceneRoot;
					sceneRoot = nullptr;
					
					currentProject.loadLibs();
					if(!nextScene) 
					{
						setScene(GameNode3D::nodeFromJson(currentScene));
					}
				}
				if(nextScene) 
				{
					if(sceneRoot && gameAssetsBrowser && gameAssetsBrowser->getParent() == sceneRoot)
						sceneRoot->dettachComponent(gameAssetsBrowser);
					if(sceneRoot) delete sceneRoot;
					sceneRoot = nextScene;
					sceneRoot->attachComponent(new TreeDisplay(EditorWindowNames::gameSceneTree));
					sceneRoot->attachComponent(new CameraViewport(EditorWindowNames::gameViewport));
					sceneRoot->attachComponent(new GameNodePropsWindow());
					sceneRoot->attachComponent(new ComponentsExplorer());
					sceneRoot->attachComponent(gameAssetsBrowser);
					sceneRoot->attachComponent(new SelectedObjectOutlinePostProcess());
					sceneRoot->attachComponent(new ProjectPropertiesWindow());
					nextScene = nullptr;
				}
				modulesNeedReload = false;
				//reset timer used for frame capping
				updateTimer = 0.0f;
				//
				Input::inputUpdate();
				//run game logics
				if(engineState == EngineState::GAME_PLAYING) 
				{
					sceneRoot->updateAll();
				}
				else 
				{
					sceneRoot->editorUpdate();
				}
				
				//destruct deleted objects
				GameNode::cleanUp();
				//update physics
				Physics::EasyPhysics::EasyPhysicsWorld::instance.solveCollisions();

				//render ids
				if(renderer.getActiveCamera()) 
				{
					if(idCamera->renderTexture.getWidth() != renderer.getActiveCamera()->renderTexture.getWidth() || idCamera->renderTexture.getHeight() != renderer.getActiveCamera()->renderTexture.getHeight())
					{
						idCamera->renderTexture.create(renderer.getActiveCamera()->renderTexture.getWidth(), renderer.getActiveCamera()->renderTexture.getHeight());
						idRenderer.resize(idCamera->renderTexture.getWidth(), idCamera->renderTexture.getHeight());
					}
					idCameraNode->setRelativeTransform(renderer.getActiveCamera()->getParentOfType<GameNode3D>()->getWorldTransform());
					switch(renderer.getActiveCamera()->getProjectionType()) 
					{
						case CameraProjectionType::PERSPECTIVE_CAMERA_PROJECTION:
						idCamera->setPerspectiveProjection(renderer.getActiveCamera()->getFOV(), renderer.getActiveCamera()->getAspect(), renderer.getActiveCamera()->getNear(), renderer.getActiveCamera()->getFar());
						break;
						case CameraProjectionType::ORTHO_CAMERA_PROJECTION:
						//idCamera->setOrthoProjection(renderer.getActiveCamera()->width, );
						break;
					}
					idRenderer.render(sceneRoot);
				}
				
				//render game
				renderer.render(sceneRoot);
				//render ui
				sceneRoot->GUIrender();
				FrameBuffer::unbind();

				//start ImGUi frame
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();
				uiSpace.display();
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
				//
				Graphics::display();
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

			TE_DEBUG_INFO("Loading and linking light shaders.");
			lightInit();

			TE_DEBUG_INFO("Initializing renderer.");
			renderer.resize(Graphics::window->getWindowWidth(), Graphics::window->getWindowHeight());
			renderer.setDepthTest(true);
			renderer.setDepthClear(true);
			renderer.setColorClear(true);
			renderer.setClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));

			IdRenderer::loadIdShader();
			idRenderer.setDepthTest(true);
			idRenderer.setDepthClear(true);
			idRenderer.setColorClear(true);
			idRenderer.setClearColor(vec4(0.0f, 0.0f, 0.0f, 0.0f));


			TE_DEBUG_INFO("Initializing Dear ImGUI");
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.Fonts->AddFontFromFileTTF("./res/fonts/DroidSansMono.ttf", 18.0f);
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			// Setup Platform/Renderer bindings
			ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)Graphics::window->gl_window_ptr_(), true);
			ImGui_ImplOpenGL3_Init("#version 130");
			// Setup Dear ImGui style
			ImGui::StyleColorsDark();

			ShaderProgram defaultMaterial = Resources::getShader("deferred");
			defaultMaterial.setTexture("tex", Resources::getTexture("bd.bmp").id());
			defaultMaterial.setTexture("nmap", Resources::getTexture("bn.bmp").id());
			defaultMaterial.setTexture("specmap", Resources::getTexture("bd.bmp").id());
			defaultMaterial.setUniform("color", glm::vec4(1.0f));
			defaultMaterial.setUniform("spec", glm::vec3(1.0f, 10.0f, 1.0f));
			Resources::addMaterial("Default", defaultMaterial);

			SelectedObjectOutlinePostProcess::loadShader();
			
			gameAssetsBrowser = new GameAssetsBrowser();
			gameAssetsBrowser->setPath(std::filesystem::current_path());
			gameAssetsBrowser->setRootPath(std::filesystem::current_path());
			setScene(new GameNode3D());

			idCameraNode = new GameNode3D();
			idCamera = new Camera();
			idCamera->renderTexture = Texture(GL_TEXTURE_2D, GL_R32I, GL_RED_INTEGER, GL_INT);
			idCamera->renderTexture.create(Graphics::window->getWindowWidth(), Graphics::window->getWindowHeight());
			idCameraNode->attachComponent(idCamera);
			idRenderer.setCamera(idCamera);
		}
		
		bool isServer() 
		{
			return false;
		}

		GameNode3D* getRoot() 
		{
			return sceneRoot;
		}

		void setProject(Project project) 
		{
			currentProject = project;
			setScene(GameNode3D::loadNodeFromJsonFile(currentProject.getDefaultScene()));
			gameAssetsBrowser->setPath(project.getPath()+"/GameAssets");
			gameAssetsBrowser->setRootPath(project.getPath()+"/GameAssets");
			requestModulesReload();
		}

		void setScene(GameNode3D* node) 
		{
			nextScene = node;
		}

		void play() 
		{
			if(engineState == EngineState::GAME_STOPPED)
			{
				originalSceneJson = sceneRoot->toJson();
				setScene(GameNode3D::nodeFromJson(originalSceneJson));
			}

			engineState = EngineState::GAME_PLAYING;
		}

		void pause() 
		{
			engineState = EngineState::GAME_PAUSED;
		}

		void stop() 
		{
			if(engineState != EngineState::GAME_STOPPED)
			setScene(GameNode3D::nodeFromJson(originalSceneJson));
			engineState = EngineState::GAME_STOPPED;
		}

		
	}
}