#pragma once
#include "System/TStruct.h"
#include <enet/enet.h>
namespace TealEngine 
{

	class TPacket : public TStruct
	{
	private:
		
	public:
		TPacket(uint8_t* data = nullptr, unsigned int dataSize = 0);
		TPacket(const TPacket& other);
		TPacket(TPacket& other);
		TStruct operator=(const TStruct& other);

		ENetPacket* createENetPacket(enet_uint32 packet_flags);
		TPacket operator +(TPacket second);
		TPacket operator+=(TPacket second);
	};
}