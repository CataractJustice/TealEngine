#define GLEW_STATIC
#pragma once
#include <iostream>
#define WIDTH 640
#define HEIGHT 480
#include "Core.h"
#include "Graphics/Camera.h"
#include "Graphics/Light/Light.h"
#include "Graphics/Mesh/MeshRenderer.h"
#include "Assets/Player.h"
#include "Math/randrange.h"
#include "Math/PerlinNoise.h"
#include "Physics/RigidBody.h"
#include <thread>
using namespace TealEngine;
Player* p;
Camera* cam;
//Clock clock;
Texture t, nt, ut;
SharedMesh* um;
DirectionLight* dlight;
GameNode* scene = new GameNode();

int main()
{
	Core::init();


	cam = new Camera();
	cam->setPerspectiveProjection(90.0f, Graphics::Window::getScreenHeight() / Graphics::Window::getScreenWidth(), 0.01f, 5000.0f);
	cam->resize(Graphics::Window::getScreenWidth(), Graphics::Window::getScreenHeight());
	Core::Scene::renderer.setCamera(cam);

	p = new Player();
	p->sensivity = 1.0f;
	p->addChild(cam);
	Transform pt;
	pt.translate(vec3(0.0f, 0.0f, -10.0f));
	p->setRelativeTransform(pt);
	scene->addChild(p);

	AmbientLight* alight = new AmbientLight();
	alight->color = vec4(0.4f);
	alight->SSAOarea = 1;
	alight->SSAOstrength = 0.4f;
	//scene->addChild(alight);

	dlight = new DirectionLight(96, 2);
	dlight->setCascadeRatio(0, 0.5f);
	dlight->color = vec4(1.0f);
	scene->addChild(dlight);

	ShaderProgram testShader = Resources::getShader("deffered").clone();
	testShader.setTexture("tex", Resources::getTexture("stone.png").id());
	Transform ct;
	ct.translate(vec3(-0.5f));
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
		
		//clock.update();
		Transform lightTransform = dlight->getTransform();
		lightTransform.rotate(0.1f, vec3(1.0f, 0.3f, 0.08f));
		dlight->setTransform(lightTransform);

		//fps[fpsi] = 1.0f / clock.deltaTime();
		fpsi++;
		fpsi %= fpssamples;

		if (Input::Keyboard::isKeyPressed(GLFW_KEY_I))
		{
			float s = 0.0f;
			for (auto f : fps) 
			{
				s += f;
			}
			std::cout << s / float(fpssamples) << "\n";
		}
		//world->setRenderPosition(p->getWorldTransform().getPosition());
		Core::Scene::update();
	}
}