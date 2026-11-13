#ifndef _WIN32
#include "libs/imgui/imgui.h"
#include "MemoryProfiler.h"
#include <sys/resource.h>

long long get_mem_usage()
{
	struct rusage usage;
	int ret;
	ret = getrusage(RUSAGE_SELF, &usage);
	return usage.ru_maxrss;
}

namespace TealEngine
{
	MemoryProfiler::ProfilerPoint::ProfilerPoint(MemoryProfiler* profiler, const std::string& label) 
	{
		this->profiler = profiler;
		initialMemoryUsage = get_mem_usage();
		this->label = label;
	}
	MemoryProfiler::ProfilerPoint::~ProfilerPoint() 
	{
		if(!ended)
			end();
	}
	
	void MemoryProfiler::ProfilerPoint::end() 
	{
		profiler->addMemoryUsageDelta(label,  get_mem_usage() - initialMemoryUsage);
	}

	void MemoryProfiler::display() 
	{
		
	}

	void MemoryProfiler::addMemoryUsageDelta(const std::string& label, long long delta) 
	{
		this->memoryUsageByLabel[label] += delta;
	}
} // namespace TealEngine
#endif