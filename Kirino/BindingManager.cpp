#include "BindingManager.h"

namespace Kirino
{
	BindingManager::BindingManager(unsigned int maxBindings) 
	{
		bindings = new unsigned int[maxBindings];
		max = maxBindings;
	}
	BindingManager::~BindingManager() 
	{
		delete[] bindings;
	}
	void BindingManager::bind(unsigned int index, unsigned int to) 
	{
		if(bindings[index] == to) return;
		bindings[index] = to;
		bindImpl(index, to);
	}
	void BindingManager::reset() 
	{
		for(int i = 0; i < max; i++) 
		{
			bindings[i] = -1;
		}
	}
} // namespace Kirino
