#include "Core.h"
#include "Graphics/Window.h"
#include <pthread.h>
#include <sched.h>
#include <chrono>
#include <thread>

int main(int argc, char *argv[]) 
{
	pthread_t this_thread = pthread_self();
	sched_param sch_params;
	sch_params.sched_priority = sched_get_priority_max(SCHED_FIFO);
	pthread_setschedparam(this_thread, SCHED_FIFO, &sch_params);
	
	TealEngine::Core::init(argc,argv);

	const std::chrono::duration<double> interval(1.0 / 60.0);
	while(!TealEngine::Graphics::window->shouldClose()) 
	{
		auto cycleStartTime = std::chrono::steady_clock::now();
		TealEngine::Core::update();
		auto updateTime = std::chrono::steady_clock::now() - cycleStartTime;
		auto updateFreeBudget = interval - updateTime;
		if (updateFreeBudget > std::chrono::duration<double>::zero())
		{
			std::this_thread::sleep_for(updateFreeBudget);
		}
	}
	TealEngine::Core::close();
}