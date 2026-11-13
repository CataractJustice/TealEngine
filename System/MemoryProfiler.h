#ifndef _WIN32
#include <string>
#include <chrono>
#include <map>
namespace TealEngine
{
	class MemoryProfiler 
	{
		public:
		std::map<std::string, long long> memoryUsageByLabel;
		class ProfilerPoint
		{
			private:
			MemoryProfiler* profiler;
			long long initialMemoryUsage;
			bool ended;
			std::string label;

			public: 
			void end();
			ProfilerPoint(MemoryProfiler*, const std::string& label);
			~ProfilerPoint();
		};
		public:
		void display();
		void addMemoryUsageDelta(const std::string& label, long long delta);
	};
} // namespace TealEngine
#endif