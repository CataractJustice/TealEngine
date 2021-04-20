#pragma once
#include "GameNode/Entity/Entity.h"
#include "System/TStruct.h"
namespace TealEngine 
{
	class IPrefab 
	{
	public:
		virtual Entity* operator()(bool serverSide, TStruct* params) = 0;
	};
}