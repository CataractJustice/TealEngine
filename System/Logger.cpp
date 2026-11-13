#include "Logger.h"
#include <string>
#include "memory.h"
#include "libs/imgui/imgui.h"
namespace TealEngine
{

	char 			Logger::logBuffer[2][loggerLogBufferSize];
	unsigned int 	Logger::logStart = 0;
	unsigned int 	Logger::logEnd = 0;
	char* 			Logger::logEndBuffer = Logger::logBuffer[0];
	char* 			Logger::logStartBuffer = Logger::logBuffer[0];
	unsigned int 	Logger::logEntries = 0;
	void Logger::eraseEntriesAtRange(char* buffer, unsigned int searchStart, unsigned int searchLength) 
	{
		if(logEntries == 0) 
		{
			memset(logBuffer[0], 0, loggerLogBufferSize);
			memset(logBuffer[1], 0, loggerLogBufferSize);
			return;
		}
		for(int i = searchStart; i < searchStart + searchLength; i++) 
		{
			if(buffer[i] == entryMarkerValue) 
			{
				LogEntryHeader header = *(LogEntryHeader*)(&buffer[i]);
				logEntries--;
				logStart += header.entryLength;
				if(logStart >= loggerLogBufferSize) 
				{
					logStart = 0;
					logStartBuffer = logStartBuffer == logBuffer[0] ? logBuffer[1] : logBuffer[0];
				}
				memset(&buffer[i], 0, header.entryLength);
				i += header.entryLength;
			}
		}
	}

	void Logger::pushEntry(char* buffer, unsigned int at, const LogEntryHeader& header, const std::string& log) 
	{
		//push this entry
		//copy header
		LogEntryHeader& bufferHeader = *(LogEntryHeader*)(&buffer[at]);
		bufferHeader = header;
		//copy log string
		memcpy(&buffer[at + sizeof(header)], log.data(), log.length());
		logEntries++;
	}

	void Logger::logInfo(const std::string& log) 
	{
		LogEntryHeader header;
		const unsigned int entrySize = sizeof(header) + log.length();
		header.entryLength = entrySize;
		header.entryMarker = entryMarkerValue;
		header.entryType = (unsigned int)LogEntryTypes::Info;
		//If have free space in a buffer after last entry append to buffer last entry is in
		if(loggerLogBufferSize - logEnd > entrySize) 
		{
			//erase next entries in a buffer
			eraseEntriesAtRange(logEndBuffer, logEnd, entrySize);
			pushEntry(logEndBuffer, logEnd, header, log);
			logEnd += entrySize;
		}
		//else if we have no free space write at the start of another buffer
		else 
		{
			char* swapBuffer = logEndBuffer == logBuffer[0] ? logBuffer[1] : logBuffer[0];
			eraseEntriesAtRange(swapBuffer, 0, entrySize);
			pushEntry(swapBuffer, 0, header, log);
			logEndBuffer = swapBuffer;
			logEnd = entrySize;
		}
	}
	void Logger::imguiDisplay() 
	{
		unsigned int logEntryStart = logStart;
		char* logEntryBuffer = logStartBuffer;
		for(int i = 0; i < logEntries; i++) 
		{
			if(logEntryStart >= loggerLogBufferSize) 
			{
				logEntryStart = 0;
				logEntryBuffer = logEndBuffer == logBuffer[0] ? logBuffer[1] : logBuffer[0];
			}
			const LogEntryHeader& header = *(LogEntryHeader*)(&logEntryBuffer[logEntryStart]);
			ImGui::TextUnformatted(
				&logEntryBuffer[logEntryStart + sizeof(header)], 
				&logEntryBuffer[logEntryStart + header.entryLength]
			);
			logEntryStart += header.entryLength;
		}
	}
} // namespace TealEngine
