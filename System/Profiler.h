#include <string>
#include <chrono>
#include <map>
namespace TealEngine
{
	class Profiler 
	{
		public:
		struct ProfilerRecord
		{
		public:
			ProfilerRecord();
			ProfilerRecord* back;
			std::chrono::high_resolution_clock::duration average;
			std::chrono::high_resolution_clock::time_point enter;
			std::chrono::high_resolution_clock::time_point exit;
			std::map<std::string, ProfilerRecord> records;
			void display(const std::string& label) const;
		};

		class ProfilerPoint
		{
			private:
			Profiler* profiler;
			std::chrono::high_resolution_clock::time_point enter;
			bool ended;

			public: 
			void end();
			ProfilerPoint(Profiler*, const std::string& label);
			~ProfilerPoint();
		};
		private:
		friend Profiler::ProfilerPoint::ProfilerPoint(Profiler*, const std::string& label);
		friend void Profiler::ProfilerPoint::end();
		ProfilerRecord initial;
		ProfilerRecord* current;

		public:
		Profiler();
		void display();
	};
} // namespace TealEngine
