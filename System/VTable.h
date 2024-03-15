#pragma once

inline void* methodAddressByVTableIndex(void *obj, int n) {
	void** vtablePtr = *(void***)obj;
	void* vtableAddress = vtablePtr[n];
	return vtableAddress;
}

template<typename T, typename R>
int getMethodVTableIndex(T& obj, R T::* func) {
	int i = 0;
	while(methodAddressByVTableIndex(&obj, i) != (void*)(obj.*func)) 
	{
		i++;
	}
	return i;
}

//returns true if obj overrides method func from base class
template<typename T, typename R>
bool isMethodOverriden(T& base, T* obj, R T::* func) 
{
	int index = getMethodVTableIndex(base, func);
	return (methodAddressByVTableIndex(obj, index) != methodAddressByVTableIndex(&base, index));
}