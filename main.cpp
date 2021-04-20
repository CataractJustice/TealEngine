#define GLEW_STATIC
#pragma once
#include <iostream>
#define WIDTH 640
#define HEIGHT 480
#include "Core.h"
#include "Graphics/Camera.h"
#include "Graphics/Light/Light.h"
#include "Graphics/Mesh/MeshRenderer.h"
#include "Assets/Sokoban/CameraController.h"
#include "Assets/Sokoban/SocobanLevel.h"
#include "Assets/Player.h"
#include "Graphics/GUI/Text/Text.h"
#include <thread>
using namespace TealEngine;
Camera* cam;
CameraController* cameraController;
SokobanLevel* level;
Texture t, nt, ut;
SharedMesh* um;
DirectionLight* dlight;
GameNode* scene = new GameNode();
Player* p;

#define w WALL
#define b BOX
#define p PLAYER
#define t TARGET
#define s BOX | TARGET

unsigned int sokobanmap[] =
{
	w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w,
	w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w,
	w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w,
	w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w,
	w, w, w, w, w, w, w, 0, 0, 0, w, w, w, w, w, w,
	w, w, w, w, w, t, p, b, 0, 0, w, w, w, w, w, w,
	w, w, w, w, w, w, w, 0, b, t, w, w, w, w, w, w,
	w, w, w, w, w, t, w, w, b, 0, w, w, w, w, w, w,
	w, w, w, w, w, 0, w, 0, t, 0, w, w, w, w, w, w,
	w, w, w, w, w, b, 0, s, b, b, t, w, w, w, w, w,
	w, w, w, w, w, 0, 0, 0, t, 0, 0, w, w, w, w, w,
	w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w,
	w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w,
	w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w,
	w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w,
	w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w
};

int main()
{
	Core::init();


	cam = new Camera();
	cam->setPerspectiveProjection(90.0f, Graphics::Window::getScreenHeight() / Graphics::Window::getScreenWidth(), 0.01f, 32.0f);
	cam->resize(Graphics::Window::getScreenWidth(), Graphics::Window::getScreenHeight());
	Core::Scene::renderer.setCamera(cam);
	
	//p = new Player();
	//p->sensivity = 1;
	//p->addChild(cam);
	//scene->addChild(p);

	cameraController = new CameraController();
	cameraController->addChild(cam);
	level = new SokobanLevel(16, 16);

	for (int i = 0; i < 16; i++)
		for (int j = 0; j < 16; j++)
			level->tile(ivec2(i, j)) = std::rand() % 2;
	level->loadLevel(sokobanmap);

	scene->addChild(cameraController);
	Transform t;
	t.setScale(vec3(0.5));
	t.translate(vec3(-8.0, 0.0, -8.0));
	level->setTransform(t);
	scene->addChild(level);

	AmbientLight* alight = new AmbientLight();
	alight->color = vec4(0.4f);
	alight->SSAOarea = 1;
	alight->SSAOstrength = 0.8f;
	//scene->addChild(alight);

	dlight = new DirectionLight(8192, 1, 32.0f);
	dlight->color = vec4(1.0f);
	scene->addChild(dlight);

	Text text;
	text.text = L"Suka Blyat'";
	text.font = Resources::getFont("comicz.ttf");
	text.scale = 1.5f;
	text.getTransform().set(GUI_Y_OFFSET, 0.5, HEIGHT_PERCENTS);
	//for (unsigned short i = 0; i < 4; i++)
	//{
	//	for (unsigned short j = 0; j < 4; j++)
	//	{
	//		for (unsigned short k = 0; k < 4; k++) 
	//		{
	//			//creating body and collider
	//			RigidBody* testBody = new RigidBody(1.0f, vec3(randrange(-8.5f, 8.5f), randrange(-8.5f, 105.5f), randrange(-8.5f, 8.5f)));
	//			Transform b2t;
	//			b2t.setPosition(vec3(i*4, j*4 + 5.0f, k*4));
	//			testBody->setTransform(b2t);
	//			Collider* cubeCollider = new Collider(new btBoxShape(btVector3(1.0f, 1.0f, 1.0f)));
	//			//append collider to body
	//			testBody->addChild(cubeCollider);
	//			//move center of the collider collider to center of the body;
	//			
	//			cubeCollider->setRelativeTransform(ct);
	//			//creating new cube mesh node
	//			SharedMesh* cubeMesh = new SharedMesh(*BasicMeshes::cube.sharedMesh());
	//			//creating new mesh renderer, append it to shared mesh, set shader
	//			((Mesh3DRenderer*)cubeMesh->addChild(new Mesh3DRenderer()))->setShader(&testShader);
	//			//append mesh to body
	//			testBody->addChild(cubeMesh);
	//			scene->addChild(testBody);
	//		}
	//	}
	//}
	
	////creating body and collider
	//RigidBody* testBody2 = new RigidBody(0.0f, vec3(0.0f));
	//Transform b2t;
	//b2t.setPosition(vec3(0.0f, -10.0f, 0.0f));
	//testBody2->setTransform(b2t);
	//Collider* cubeCollider2 = new Collider(new btBoxShape(btVector3(100.0f, 1.0f, 100.0f)));
	////append collider to body
	//testBody2->addChild(cubeCollider2);
	//cubeCollider2->setRelativeTransform(ct);
	////creating new cube mesh node
	//SharedMesh* cubeMesh2 = new SharedMesh(*BasicMeshes::cube.sharedMesh());
	//Transform cmt;
	//cmt.setScale(vec3(100.0f, 1.0f, 100.0f));
	//cubeMesh2->setTransform(cmt);
	////creating new mesh renderer, append it to shared mesh, set shader
	//((Mesh3DRenderer*)cubeMesh2->addChild(new Mesh3DRenderer()))->setShader(&testShader);
	////append mesh to body
	//cubeCollider2->addChild(cubeMesh2);
	//scene->addChild(testBody2);
	
	Core::Scene::addNode(scene);
	
	
	#define fpssamples 38
	float fps[fpssamples];
	unsigned short fpsi = 0;

	while (!glfwWindowShouldClose(Graphics::Window::window)) 
	{
		if(Input::Keyboard::isKeyPressed(GLFW_KEY_R))
			level->loadLevel(sokobanmap);
		
		//clock.update();
		Transform lightTransform;
		lightTransform.rotate(glfwGetTime() * 10.0f, vec3(0.0f, 1.0f, 0.0f));
		lightTransform.rotate(90.0f, vec3(1.0f, 0.1f, 1.08f));
		dlight->setTransform(lightTransform);

		//fps[fpsi] = 1.0f / clock.deltaTime();
		fpsi++;
		fpsi %= fpssamples;

		if (Input::Keyboard::isKeyPressed(GLFW_KEY_I))
		{
			float sec = 0.0f;
			for (auto f : fps) 
			{
				sec += f;
			}
			std::cout << sec / float(fpssamples) << "\n";
		}
		//world->setRenderPosition(p->getWorldTransform().getPosition());
		Core::Scene::update();
	}
}