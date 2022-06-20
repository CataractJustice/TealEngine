#pragma once
#include "TPacket.h"
#include "enet/enet.h"
#include "libs/compression/huffman.hpp"
namespace TealEngine
{
	TPacket::TPacket(char* data, unsigned int dataSize, bool decompress) : TStruct(data, dataSize, decompress) {};////..
	TPacket::TPacket(const TPacket& other) : TStruct(other) {};
	TPacket::TPacket(TPacket& other) : TStruct(other) {};
	TStruct TPacket::operator=(const TStruct& other) { return TStruct::operator=(other); }

	void* TPacket::createENetPacket(unsigned int packet_flags)
	{
		unsigned int messageSize = 0;
		uint8_t* uncompressedMessage = constructDataString(messageSize);
		
		uint8_t* compressedMessage;
		int compressedSize, compressedSizeBits;
		huffman::easyEncode(uncompressedMessage, messageSize, &compressedMessage, &compressedSize, &compressedSizeBits);
		uint8_t* finalMessage = new uint8_t[sizeof(unsigned int) * 2 + compressedSize]; //change da world
		memcpy(finalMessage, &messageSize, sizeof(unsigned int));
		memcpy(finalMessage + sizeof(unsigned int), &compressedSizeBits, sizeof(unsigned int));
		memcpy(finalMessage + sizeof(unsigned int) * 2, compressedMessage, compressedSize);
		ENetPacket* packet = enet_packet_create(finalMessage, compressedSize + sizeof(unsigned int) * 2, packet_flags);
		HUFFMAN_MFREE(compressedMessage);
		delete[] finalMessage;
		return packet;
	}

	TPacket TPacket::operator +(TPacket second)
	{
		TPacket first(*this);
		for (unsigned int i = 0; i < second.getFieldsCount(); i++)
			first.push(second.getString(i));
		return first;
	}

	TPacket TPacket::operator+=(TPacket second)
	{
		for (unsigned int i = 0; i < second.getFieldsCount(); i++)
			this->push(second.getString(i));
		return *this;
	}
	
}