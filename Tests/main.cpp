#include "Core.h"
#include "Tests/GameNodeTests/GameNodeTreeTests.h"
using namespace TealEngine;

int main(int argc, char** argv) 
{
	Core::init();
	Core::play();
	Core::update();
	GameNodeTreeTests gameNodeTreeTests;
	
	gameNodeTreeTests.run();

	std::cout << "\nTests completed\n";
}