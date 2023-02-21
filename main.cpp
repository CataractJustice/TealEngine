#include "Core.h"
#include "Graphics/Window.h"
int main() 
{
	TealEngine::Core::init();

	while(!TealEngine::Graphics::window->shouldClose()) 
	{
		TealEngine::Core::update();
	}
}