#pragma once
#define TE_DEBUG
#include <iostream>
#include <string>
using namespace std;


#define TE_NO_DEBUG

namespace TealEngine 
{
	namespace Debug 
	{
#ifndef TE_NO_DEBUG
		#define TE_DEBUG_LOG(log)\
			{TealEngine::Debug::logStr += string(log) + "\nLine: " + to_string(__LINE__) + ";\nFile:" + __FILE__ + ";\n";\
			std::cout << string(log) << "\nLine: " + to_string(__LINE__) << ";\nFile:" << __FILE__ << ";\n";}

		#define TE_DEBUG_WARNING(log) TE_DEBUG_LOG(string("WARNING: ") + log);
		#define TE_DEBUG_ERROR(log) TE_DEBUG_LOG(string("ERROR: ") + log);
		#define TE_NOT_IMPLEMENTED() TE_DEBUG_ERROR(string("Not implemented yet."));
		#define TE_DEBUG_INFO(log) TE_DEBUG_LOG(string("INFO: ") + log);
#else
	#define TE_DEBUG_LOG(log)
	#define TE_DEBUG_WARNING(log)
	#define TE_DEBUG_ERROR(log)
	#define TE_NOT_IMPLEMENTED()
	#define TE_DEBUG_INFO()
#endif
		extern std::string logStr;
		extern void saveLog();

	}
}