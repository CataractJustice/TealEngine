#pragma once
#include "../Entity/Entity.h"
#include "../../System/TStruct.h"
namespace TealEngine 
{
	class Prefab 
	{
	public:
		static Entity* create(bool serverSide, TStruct* params);
	};
}