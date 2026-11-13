#include <map>
#include <string>
#include "Core.h"
#include <fstream>
#include "FileSystem/loadStrFromFile.h"
#include "Graphics/Graphics.h"
#include "Graphics/Mesh/MeshUtil.h"
#include "Graphics/Renderer/RenderUtil.h"
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
#include "Editor/UI/EditorUI/GameAssetsBrowser.h"
#include "Editor/UI/SelectedObjectOutlinePostProcess.h"
#include "Graphics/GUI/Text/FreeTypeUtil.h"
#include "GameNode/SceneNode.h"
#include "Graphics/DeferredRenderer.h"
#include "Graphics/Light/Light.h"
#include "Graphics/Shapes/Line3D.h"
#include "enet/enet.h"
#ifndef _WIN32
#include <sys/resource.h>
#endif
using Json = nlohmann::json;

namespace TealEngine
{
	namespace Core
	{
		SceneNode* scene;
		GameNode* sceneRoot;
		Clock sceneclock;
		UISpace uiSpace;
		GameNode3D* idCameraNode;
		Camera* idCamera;
		const Camera* camera;
		ActionList actionList;
		Project currentProject;
		TextureManager textureManager;
		ModelsManager modelsManager;
		ShadersManager shadersManager;
		MaterialsManager materialsManager;
		FontManager fontManager;
		PhysicsScene* physicsScene;
		Profiler profiler;
		Kirino::UniformBufferObject cameraUBO;
		Kirino::UniformBufferObject transformUBO;
		DeferredRenderer* renderer;
		ShapesRenderer shapesRenderer;
		Client client;
		Host host;
		NetworkDictionary networkDictionary;
		LaunchArguments launchArguments;
		GameNode* originalScene;
		GameNode* nextScene = nullptr;
		Json originalSceneJson;
		
		int frameId = 0;

		Kirino::Material imageMaterial;


		bool modulesNeedReload = false;
		void requestModulesReload() { modulesNeedReload = true; }

		std::string nextScenePath = "";

		EngineState engineState = EngineState::GAME_STOPPED;
		EngineState targetEngineState = EngineState::GAME_STOPPED;

		unsigned int getFrameId() 
		{
			return frameId;
		}

		void setCamera(const Camera* cam) 
		{
			camera = cam;
			
			struct alignas(16) UBOData 
			{
				glm::mat4 PV;
				glm::vec3 dir;
				float p0;
				glm::vec3 pos;
				float p1;
			};
			UBOData ubo;
			ubo.PV = cam->getPV();
			ubo.dir = cam->getParentOfTypeConst<GameNode3D>()->getWorldTransform().forward();
			ubo.pos = cam->getParentOfTypeConst<GameNode3D>()->getWorldTransform().getPosition();
			cameraUBO.upload(ubo);
		}

		void playImpl() 
		{
			originalScene = sceneRoot;
			sceneRoot = GameNode::nodeFromJson(sceneRoot->toJson());
			scene->removeChild(originalScene);
			scene->addChild(sceneRoot);
		}

		void stopImpl() 
		{
			scene->removeChild(sceneRoot);
			sceneRoot->destroy();
			GameNode::cleanupDestroyed();
			sceneRoot = originalScene;
			scene->addChild(sceneRoot);
		}

		void update()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			frameId++;
			Profiler::ProfilerPoint frameProf(&profiler, "Frame");
			static float updateTimer = 0.0f;
			sceneclock.update();
			auto frameStart = std::chrono::high_resolution_clock::now();
			Profiler::ProfilerPoint renderProf(&profiler, "Render");
			//render ids
			Profiler::ProfilerPoint idsProf(&profiler, "Ids");
			/*
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
			}*/
			
			idsProf.end();
			//render game
			Profiler::ProfilerPoint defferedProf(&profiler, "Deferred render");
			renderer->render(Camera::getPrimary());
			defferedProf.end();
			Profiler::ProfilerPoint guiProf(&profiler, "Engine GUI");
			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);
			scene->refreshGUI();
			//Display render texture on screen
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0,0, Graphics::window->getWindowWidth(), Graphics::window->getWindowHeight());
			
			if(Camera::getPrimary()) 
			{
				shapesRenderer.pushShape(Line3D(glm::vec3(0.0f), glm::vec3(4.0f, 0.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.2f));
				shapesRenderer.pushShape(Line3D(glm::vec3(0.0f), glm::vec3(0.0f, 4.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 0.2f));
				shapesRenderer.pushShape(Line3D(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 4.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.2f));
				setCamera(Camera::getPrimary());
				shapesRenderer.renderAll();
			}
			//Render GUI
			//sceneRoot->GUIRender();
			guiProf.end();
			renderProf.end();

			Profiler::ProfilerPoint updateProf(&profiler, "Update");
			//update modules
			if(modulesNeedReload) 
			{
				reloadModules();
				modulesNeedReload = false;
			}
			//Switch scene
			if(nextScene) 
			{
				scene->removeChild(sceneRoot);
				delete sceneRoot;
				sceneRoot = nextScene;
				scene->addChild(sceneRoot);
				nextScene = nullptr;
			}
			else if(nextScenePath.length()) 
			{
				scene->removeChild(sceneRoot);
				delete sceneRoot;
				sceneRoot = GameNode::loadNodeFromJsonFile(nextScenePath);
				scene->addChild(sceneRoot);
				nextScenePath = "";
			}
			//Update engine state
			if(engineState == EngineState::GAME_STOPPED && targetEngineState != EngineState::GAME_STOPPED) 
			{
				//we need engine state to be "GAME_PLAYING" so that onAttach and other play only callbacks get called
				engineState = EngineState::GAME_PLAYING;
				playImpl();
			}
			else if(engineState != EngineState::GAME_STOPPED && targetEngineState == EngineState::GAME_STOPPED) 
			{
				stopImpl();
			}
			engineState = targetEngineState;
			Input::inputUpdate();
			
			scene->editorUpdate();
			//run game logics
			if(engineState == EngineState::GAME_PLAYING) 
			{
				scene->update();
				physicsScene->collisionsCheck();
				physicsScene->collisionsFlush();
			}
			//destruct objects marked as destroyed
			GameNode::cleanupDestroyed();
			updateProf.end();
			//ImGUI
			Profiler::ProfilerPoint imGuiProf(&profiler, "ImGui GUI");
			//Editor ui
			uiSpace.display();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			imGuiProf.end();
			//Swap buffers and other
			Profiler::ProfilerPoint displayProf(&profiler, "Display");
			Graphics::display();
			displayProf.end();
			if(host.isLive()) host.update(scene);
			if(client.isLive()) client.update(scene);
		}

		void windowResizeCallback(int width, int height) 
		{
			scene->windowResize(width, height);
		}

		void init(int argc, char** argv)
		{
			launchArguments.parse(argc, argv);
			//load settings
			std::fstream settingsFile("settings.json");
			Json settingsJson = Json::parse(settingsFile);
			//initialise systems
			Graphics::init(settingsJson["title"]);
			BasicMeshes::init();
			Input::init();
			//lightInit();
			uiSpace.init();
			FreeTypeUtil::FreeTypeInitialize();

			Graphics::window->setWindowResizeCallback(windowResizeCallback);
			enet_initialize();
			//default UBOs 
			cameraUBO.create();
			transformUBO.create();
			materialsManager.addDefaultUBO("CameraBlock", cameraUBO.getId());
			materialsManager.addDefaultUBO("TransformBlock", transformUBO.getId());

			shadersManager.loadRecursive("./Assets");
			modelsManager.loadRecursive("./Assets");
			textureManager.loadRecursive("./Assets");
			materialsManager.loadRecursive("./Assets");
			fontManager.loadRecursive("./Assets");
			
			//ImGUI
			IMGUI_CHECKVERSION();
			
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			//io.Fonts->AddFontFromFileTTF("./Assets/Fonts/DroidSansMono.ttf", 18.0f);
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			ImVec4* colors = ImGui::GetStyle().Colors;
  			ImGuiStyle& style = ImGui::GetStyle();
  			style.Alpha = 1.0f;
        	style.Colors[ImGuiCol_Text]						= ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			style.Colors[ImGuiCol_TextDisabled]				= ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
			style.Colors[ImGuiCol_WindowBg]					= ImVec4(0.13f, 0.14f, 0.15f, 0.20f);
			style.Colors[ImGuiCol_ChildBg]					= ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
			style.Colors[ImGuiCol_PopupBg]					= ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
			style.Colors[ImGuiCol_Border]					= ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
			style.Colors[ImGuiCol_BorderShadow]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_FrameBg]					= ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
			style.Colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
			style.Colors[ImGuiCol_FrameBgActive]			= ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
			style.Colors[ImGuiCol_TitleBg]					= ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
			style.Colors[ImGuiCol_TitleBgActive]			= ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
			style.Colors[ImGuiCol_TitleBgCollapsed]			= ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
			style.Colors[ImGuiCol_MenuBarBg]				= ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarBg]				= ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
			style.Colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered]		= ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
			style.Colors[ImGuiCol_CheckMark]				= ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
			style.Colors[ImGuiCol_SliderGrab]				= ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
			style.Colors[ImGuiCol_SliderGrabActive]			= ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
			style.Colors[ImGuiCol_Button]					= ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
			style.Colors[ImGuiCol_ButtonHovered]			= ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
			style.Colors[ImGuiCol_ButtonActive]				= ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
			style.Colors[ImGuiCol_Header]					= ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
			style.Colors[ImGuiCol_HeaderHovered]			= ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
			style.Colors[ImGuiCol_HeaderActive]				= ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
			style.Colors[ImGuiCol_Separator]				= style.Colors[ImGuiCol_Border];
			style.Colors[ImGuiCol_SeparatorHovered]			= ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
			style.Colors[ImGuiCol_SeparatorActive]			= ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			style.Colors[ImGuiCol_ResizeGrip]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_ResizeGripHovered]		= ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
			style.Colors[ImGuiCol_ResizeGripActive]			= ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			style.Colors[ImGuiCol_Tab]						= ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
			style.Colors[ImGuiCol_TabHovered]				= ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
			style.Colors[ImGuiCol_TabActive]				= ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
			style.Colors[ImGuiCol_TabUnfocused]				= ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
			style.Colors[ImGuiCol_TabUnfocusedActive]		= ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
			style.Colors[ImGuiCol_DockingPreview]			= ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
			style.Colors[ImGuiCol_DockingEmptyBg]			= ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
			style.Colors[ImGuiCol_PlotLines]				= ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			style.Colors[ImGuiCol_PlotLinesHovered]			= ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram]			= ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogramHovered]		= ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg]			= ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			style.Colors[ImGuiCol_DragDropTarget]			= ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
			style.Colors[ImGuiCol_NavHighlight]				= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			style.Colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			style.Colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			style.Colors[ImGuiCol_ModalWindowDimBg]			= ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
			style.Colors[ImGuiCol_DockingEmptyBg]			= ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			style.GrabRounding								= style.FrameRounding = 2.3f;

			// Setup Platform/Renderer bindings
			ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)Graphics::window->gl_window_ptr_(), true);
			ImGui_ImplOpenGL3_Init("#version 130");
			
			SelectedObjectOutlinePostProcess::loadShader();
			gameAssetsBrowser->setPath(std::filesystem::current_path());
			gameAssetsBrowser->setRootPath(std::filesystem::current_path());
			scene = new SceneNode();
			renderer = new DeferredRenderer();
			physicsScene = new PhysicsScene();
			scene->attachComponents({physicsScene, renderer});
			sceneRoot = new GameNode3D();
			scene->addChild(sceneRoot);
			if(launchArguments.exists("p")) 
			{
				std::string projectPathArg = launchArguments.get("p");
				Core::setProject(Project::open(projectPathArg));
			}
			if(launchArguments.exists("run")) 
			{
				play();
			}
		}

		GameNode* getRoot() 
		{
			return sceneRoot;
		}

		void setProject(Project project) 
		{
			if(currentProject.isOpen()) 
			{
				textureManager.unloadRecursive(currentProject.getPath()+"/GameAssets");
				modelsManager.unloadRecursive(currentProject.getPath()+"/GameAssets");
				shadersManager.unloadRecursive(currentProject.getPath()+"/GameAssets");
				materialsManager.unloadRecursive(currentProject.getPath()+"/GameAssets");
				fontManager.unloadRecursive(currentProject.getPath()+"/GameAssets");
			}
			currentProject = project;
			requestModulesReload();
			setScene(currentProject.getDefaultScene());
			gameAssetsBrowser->setPath(project.getPath()+"/GameAssets");
			gameAssetsBrowser->setRootPath(project.getPath()+"/GameAssets");
			textureManager.loadRecursive(project.getPath()+"/GameAssets");

			modelsManager.loadRecursive(project.getPath()+"/GameAssets");
			modelsManager.build();
			shadersManager.loadRecursive(project.getPath()+"/GameAssets");
			materialsManager.loadRecursive(project.getPath()+"/GameAssets");
			fontManager.loadRecursive(project.getPath()+"/GameAssets");

			std::filesystem::current_path(project.getPath());
			imageMaterial.setProgram(&shadersManager.get("ScreenImage"));
			unsigned int backgroundTexture = textureManager.get("RGBNoise.png").id();
			imageMaterial.setTexture("textureImage", &backgroundTexture, 1, 0);
			lightInit();
			networkDictionary.load();
		}

		void setScene(GameNode* node) 
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
			Json currentScene;
			if(sceneRoot) 
			{
				//save current scene to restore after new libs are loaded
				currentScene = sceneRoot->toJson();
				scene->removeChild(sceneRoot);
				//delete scene while old libs are still loaded so proper destructors are called
				delete sceneRoot;
			}

			currentProject.loadLibs();
			//restore scene with new libs loaded
			if(sceneRoot) 
			{
				sceneRoot = GameNode::nodeFromJson(currentScene);
				scene->addChild(sceneRoot);
			}
		}
		SceneNode* sceneNode() 
		{
			return scene;
		}

		void close() 
		{
			delete scene;
			if(scene != originalScene) delete originalScene;
		}
	}
}