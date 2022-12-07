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
#include <thread>
using namespace TealEngine;
Camera* cam;
Texture t, nt, ut;
SharedMesh* um;
DirectionLight* dlight;
GameNode* scene = new GameNode3D();

int main()
{
	std::cout << "Hello";
}