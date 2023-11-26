#include <map>
#include <string>
#include "Core.h"
#include <fstream>
#include "FileSystem/loadStrFromFile.h"
#include "Graphics/Graphics.h"
#include "Graphics/Mesh/MeshUtil.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "Graphics/Renderer/DeferredRenderer.h"
#include "Graphics/Mesh/MeshRenderer.h"
#include "System/Clock.h"
#include <thread>
#include "DefaultTags.h"
#include "System/Debug.h"
#include "libs/imgui/imgui.h"
#include "libs/imgui/backends/imgui_impl_opengl3.h"
#include "libs/imgui/backends/imgui_impl_glfw.h"
#include "NlohmannJson/json.hpp"
#include "System/DLLoader.h"
#include "GameNode/ComponentFactory.h"
#include "Editor/UI/UISpace.h"
#include "Graphics/Renderer/IdRenderer.h"
#include "Editor/UI/EditorUI/GameAssetsBrowser.h"
#include "Graphics/Renderer/IdRenderer.h"
#include "Editor/UI/SelectedObjectOutlinePostProcess.h"

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
		TextureManager textureManager;
		ModelsManager modelsManager;
		ShadersManager shadersManager;
		MaterialsManager materialsManager;
		PhysicsScene physicsScene;
		ShapesRenderer shapesRenderer;

		Json originalSceneJson;
		GameNode3D* originalScene;

		bool modulesNeedReload = false;
		void requestModulesReload() { modulesNeedReload = true; }

		GameNode3D* nextScene = nullptr;
		std::string nextScenePath = "";

		EngineState engineState = EngineState::GAME_STOPPED;
		EngineState targetEngineState = EngineState::GAME_STOPPED;

		void playImpl() 
		{
			originalScene = sceneRoot;
			sceneRoot = GameNode3D::nodeFromJson(sceneRoot->toJson());
		}

		void stopImpl() 
		{
			delete sceneRoot;
			sceneRoot = originalScene;
		}

		void update()
		{
			static float updateTimer = 0.0f;
			sceneclock.update();
			updateTimer += sceneclock.deltaTime();
			if(true) 
			{
				//update modules
				if(modulesNeedReload) 
				{
					reloadModules();
					modulesNeedReload = false;
				}
				//Switch scene
				if(nextScene) 
				{
					delete sceneRoot;
					sceneRoot = nextScene;
					nextScene = nullptr;
				}
				else if(nextScenePath.length()) 
				{
					delete sceneRoot;
					sceneRoot = GameNode3D::loadNodeFromJsonFile(nextScenePath);
					nextScenePath = "";
				}
				//reset timer used for frame capping
				updateTimer = 0.0f;
				//Update engine state
				if(engineState == EngineState::GAME_STOPPED && targetEngineState != EngineState::GAME_STOPPED) 
				{
					playImpl();
				}
				else if(engineState != EngineState::GAME_STOPPED && targetEngineState == EngineState::GAME_STOPPED) 
				{
					stopImpl();
				}
				engineState = targetEngineState;

				Input::inputUpdate();
				//run game logics
				if(engineState == EngineState::GAME_PLAYING) 
				{
					sceneRoot->updateAll();
					physicsScene.CollisionCheck();
					physicsScene.CollisionFlush();
				}
				else 
				{
					sceneRoot->editorUpdate();
				}
				
				//destruct objects marked as destroyed
				GameNode::cleanupDestroyed();

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

				//
				glDisable(GL_CULL_FACE);
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_BLEND);
				//Display render texture on screen
				FrameBuffer::unbind();
				glViewport(0,0, Graphics::window->getScreenWidth(), Graphics::window->getScreenHeight());
				if(renderer.getActiveCamera())
					Render::renderTexture(renderer.getActiveCamera()->renderTexture.id());
				//Render GUI
				sceneRoot->GUIrender();

				//ImGUI
				//start ImGUi frame
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();
				//Editor ui
				uiSpace.display();
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
				//Swap buffers and other
				Graphics::display();
			}
		}

		void init()
		{
			//load settings
			std::fstream settingsFile("settings.json");
			Json settingsJson = Json::parse(settingsFile);
			//initialise systems
			Graphics::init(settingsJson["title"]);
			BasicMeshes::init();
			Input::init();
			lightInit();
			uiSpace.init();
			
			//setup deferred renderer
			renderer.resize(Graphics::window->getWindowWidth(), Graphics::window->getWindowHeight());
			renderer.setDepthTest(true);
			renderer.setDepthClear(true);
			renderer.setColorClear(true);
			renderer.setClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
			//setup id renderer
			IdRenderer::loadIdShader();
			idRenderer.setDepthTest(true);
			idRenderer.setDepthClear(true);
			idRenderer.setColorClear(true);
			idRenderer.setClearColor(vec4(0.0f, 0.0f, 0.0f, 0.0f));

			//ImGUI
			IMGUI_CHECKVERSION();
			
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.Fonts->AddFontFromFileTTF("./res/fonts/DroidSansMono.ttf", 18.0f);
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			ImVec4* colors = ImGui::GetStyle().Colors;
  ImGuiStyle& style = ImGui::GetStyle();
  		style.Alpha = 1.0f;
        style.Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.15f, 0.20f);
style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
style.Colors[ImGuiCol_Border]                = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
style.Colors[ImGuiCol_Button]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
style.Colors[ImGuiCol_Header]                = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
style.Colors[ImGuiCol_Separator]             = style.Colors[ImGuiCol_Border];
style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
style.Colors[ImGuiCol_Tab]                   = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
style.Colors[ImGuiCol_TabActive]             = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
style.Colors[ImGuiCol_DockingPreview]        = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
style.Colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
style.Colors[ImGuiCol_DockingEmptyBg] 		 = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
style.GrabRounding                           = style.FrameRounding = 2.3f;

			// Setup Platform/Renderer bindings
			ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)Graphics::window->gl_window_ptr_(), true);
			ImGui_ImplOpenGL3_Init("#version 130");
			
			SelectedObjectOutlinePostProcess::loadShader();
			gameAssetsBrowser->setPath(std::filesystem::current_path());
			gameAssetsBrowser->setRootPath(std::filesystem::current_path());
			sceneRoot = new GameNode3D();

			idCameraNode = new GameNode3D();
			idCamera = new Camera();
			idCamera->renderTexture = Texture(GL_TEXTURE_2D, GL_R32I, GL_RED_INTEGER, GL_INT);
			idCamera->renderTexture.create(Graphics::window->getWindowWidth(), Graphics::window->getWindowHeight());
			idCameraNode->attachComponent(idCamera);
			idRenderer.setCamera(idCamera);
		}

		GameNode3D* getRoot() 
		{
			return sceneRoot;
		}

		void setProject(Project project) 
		{
			currentProject = project;
			requestModulesReload();
			setScene(currentProject.getDefaultScene());
			gameAssetsBrowser->setPath(project.getPath()+"/GameAssets");
			gameAssetsBrowser->setRootPath(project.getPath()+"/GameAssets");
			textureManager.clearAll();
			textureManager.loadRecursive(project.getPath()+"/Assets");
			textureManager.loadRecursive(project.getPath()+"/GameAssets");
			modelsManager.clearAll();
			modelsManager.loadRecursive(project.getPath()+"/Assets");
			modelsManager.loadRecursive(project.getPath()+"/GameAssets");
			shadersManager.clearAll();
			shadersManager.loadRecursive(project.getPath()+"/Assets");
			shadersManager.loadRecursive(project.getPath()+"/GameAssets");
			materialsManager.clearAll();
			materialsManager.loadRecursive(project.getPath()+"/Assets");
			materialsManager.loadRecursive(project.getPath()+"/GameAssets");
			std::filesystem::current_path(project.getPath());
		}

		void setScene(GameNode3D* node) 
		{
			nextScene = node;
		}

		void setScene(const std::string& scenePath) 
		{
			nextScenePath = scenePath;
		}

		void play()
		{
			targetEngineState = EngineState::GAME_PLAYING;
		}

		void pause() 
		{
			targetEngineState = EngineState::GAME_PAUSED;
		}

		void stop() 
		{
			targetEngineState = EngineState::GAME_STOPPED;
		}

		EngineState getEngineState() 
		{
			return engineState;
		}
	
		void reloadModules() 
		{
			currentProject.buildLibs();

			Json currentScene;
			if(sceneRoot) 
			{
				//save current scene to restore after new libs are loaded
				currentScene = sceneRoot->toJson();
				//delete scene while old libs are still loaded so proper destructors are called
				delete sceneRoot;
			}

			currentProject.loadLibs();
			//restore scene with new libs loaded
			if(sceneRoot)
				sceneRoot = GameNode3D::nodeFromJson(currentScene);
		}
	}
}