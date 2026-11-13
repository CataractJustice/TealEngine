#include <string>
namespace TealEngine
{
	static const unsigned char entryMarkerValue = 255;
	enum class LogEntryTypes 
	{
		Info,
		Error
	};
	#pragma pack(push, 1)
	struct LogEntryHeader 
	{
		unsigned char entryMarker;
		unsigned int entryLength;
		unsigned int entryType;
	};
	#pragma pack(pop)
	static const unsigned int loggerLogBufferSize = 16777216;
	class Logger 
	{
		private:
		static char logBuffer[2][loggerLogBufferSize];
		static unsigned int logStart;
		static unsigned int logEnd;
		static char* logEndBuffer;
		static char* logStartBuffer;
		static unsigned int logEntries;
		static void eraseEntriesAtRange(char* buffer, unsigned int searchStart, unsigned int searchLength);
		static void pushEntry(char* buffer, unsigned int at, const LogEntryHeader& header, const std::string& log);
		public:
		static void logInfo(const std::string& log);
		static void imguiDisplay();
	};
} // namespace TealEngine
