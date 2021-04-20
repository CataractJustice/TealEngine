#pragma once
#include "System/TStruct.h"
namespace TealEngine 
{

	class TPacket : public TStruct
	{
	private:
		
	public:
		TPacket(char* data = nullptr, unsigned int dataSize = 0);
		TPacket(const TPacket& other);
		TPacket(TPacket& other);
		TStruct operator=(const TStruct& other);

		void* createENetPacket(unsigned int packet_flags);
		TPacket operator +(TPacket second);
		TPacket operator+=(TPacket second);
	};
}