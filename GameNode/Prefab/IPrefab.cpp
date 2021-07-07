#include "IPrefab.h"

namespace TealEngine 
{
	void IPrefab::setParams(TStruct* params, bool makeCopy, bool deleteOnDestructor) 
	{
		if (makeCopy)
			*this->params = *params;
		else
			this->params = params;
		deleteParamsOnDestructor = deleteOnDestructor;
	}

	IPrefab::~IPrefab() 
	{
		if (deleteParamsOnDestructor) delete params;
	}
}