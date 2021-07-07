#pragma once
#include "GameNode/Entity/Entity.h"
#include "System/TStruct.h"
namespace TealEngine 
{
	class IPrefab 
	{
	protected:
		TStruct* params;
	private:
		bool deleteParamsOnDestructor;
	public:
		virtual Entity* operator()(bool serverSide) = 0;
		void setParams(TStruct* params, bool makeCopy, bool deleteOnDestructor);
		~IPrefab();
	};
}