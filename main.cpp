#define GLEW_STATIC
#pragma once
#include <iostream>
#define WIDTH 640
#define HEIGHT 480
#include "Core.h"
#include "Graphics/Camera.h"
#include "Graphics/Light/Light.h"
#include "Graphics/Mesh/MeshRenderer.h"
#include "Graphics/GUI/Text/Text.h"
#include "Assets/FreeCam.h"
#include "Assets/Voxel/ClientSideVoxelWorld.h"
#include <thread>
using namespace TealEngine;
Camera* cam;
Texture t, nt, ut;
SharedMesh* um;
DirectionLight* dlight;
GameNode* scene = new GameNode3D();

int main()
{
	Core::init();

	ShaderProgram defaultmat = Resources::getShader("deffered");

	defaultmat.setTexture("tex", Resources::getTexture("fighterbase.bmp").id());
	defaultmat.setTexture("nmap", Resources::getTexture("bn.bmp").id());
	defaultmat.setUniform("color", glm::vec4(1.0f));
	Resources::addMaterial("default", defaultmat);

	cam = new Camera();
	cam->setPerspectiveProjection(90.0f, Graphics::window->getAspect(), 0.01f, 320.0f);
	cam->resize(Graphics::window->getScreenWidth(), Graphics::window->getWindowHeight());
	Core::Scene::renderer.setCamera(cam);
	FreeCam* camcontroller = new FreeCam();
	camcontroller->sensitivity = 0.01f;
	cam->attachComponent(camcontroller);
	scene->addChild(cam);
	
	dlight = new DirectionLight   (2048, 5, 32.0f);
	dlight->color = vec4(1.0f);
	dlight->setCascadeRatio(0, 1.0f);
	dlight->setCascadeRatio(1, 2.0f);
	dlight->setCascadeRatio(2, 4.0f);
	dlight->setCascadeRatio(3, 8.0f);
	dlight->setCascadeRatio(4, 16.0f);

	scene->attachComponent(dlight);

	//Text text;
	//text.font = &Resources::getFont("comici.ttf");
	//text.getTransform().set(GUI_Y_OFFSET, 0.0, HEIGHT_PERCENTS);
	//text.getTransform().set(GUI_X_OFFSET, 0.0, HEIGHT_PERCENTS);
	//text.setScale(0.01f);
	//scene->addChild(&text);


	GameNode3D* cube = new GameNode3D();
	Mesh3DRenderer* mr = new Mesh3DRenderer();
	mr->setMesh(Resources::getModel("untitled.obj")->sharedMesh());
	mr->setShader(&Resources::getMaterial("default"));
	cube->attachComponent(mr);
	cube->getRelativeTransform().translate(glm::vec3(0.0f, 0.0f, 15.0f));
	scene->addChild(cube);

	ClientSideVoxelWorld* vw = new ClientSideVoxelWorld(8, glm::ivec3(32), &Resources::getMaterial("default"), 1); 
	GameNode3D* vwnode = new GameNode3D();
	vwnode->attachComponent(vw);
	vw->setRenderPosition(glm::vec3(32.0f));

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 8; k++) {
				VoxelData* chunk = new VoxelData(glm::ivec3(32));
				for (int i1 = 0; i1 < 32; i1++) {
					for (int j1 = 0; j1 < 32; j1++) {
						for (int k1 = 0; k1 < 32; k1++) {
							Voxel voxel;
							voxel.amount = 1.0f - glm::length(glm::vec3(i1 - 16, j1 - 16, k1 - 16) / 16.0f);
							chunk->quickSetVoxel(glm::ivec3(i1, j1, k1), voxel);
						}
					}
				}
				vw->loadChunk(glm::ivec3(i,j,k), chunk);
			}
		}
	}

	
	scene->addChild(vwnode);
	Core::Scene::addNode(scene);

	while (!Graphics::window->shouldClose())
	{
		Transform lightTransform;
		//text.setText(std::to_wstring(cam->getRelativeTransform().getX()) + L"  " + std::to_wstring(cam->getRelativeTransform().getY()));
		lightTransform.rotate(glfwGetTime() * 10.0f, vec3(0.0f, 1.0f, 0.0f));
		lightTransform.rotate(90.0f, vec3(1.0f, 0.1f, 1.08f));
		Core::Scene::update();
	}
}