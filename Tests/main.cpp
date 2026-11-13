#include "Core.h"
#include "Tests/GameNodeTests/GameNodeTreeTests.h"
#include <iostream>
using namespace TealEngine;

int main(int argc, char** argv) 
{
	Core::init(argc,argv);
	Core::play();
	Core::update();
	GameNodeTreeTests gameNodeTreeTests;
	
	gameNodeTreeTests.run();

	std::cout << "\nTests completed\n";
}