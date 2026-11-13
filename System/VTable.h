#pragma once

__attribute__((optimize("O0"))) inline void** getVTablePtr(void* obj) 
{
	return *(void***)obj;
}

__attribute__((optimize("O0"))) inline void* methodAddressByVTableIndex(void *obj, int n) {
	void** vtablePtr = getVTablePtr(obj);
	void* vtableAddress = vtablePtr[n];
	return vtableAddress;
}

template<typename T, typename R>
__attribute__((optimize("O0"))) int getMethodVTableIndex(T& obj, R T::* func) {
	int i = 0;
	while(methodAddressByVTableIndex(&obj, i) != (void*)(obj.*func)) 
	{
		i++;
	}
	return i;
}

//returns true if obj overrides method func from base class
template<typename T, typename R>
__attribute__((optimize("O0"))) bool isMethodOverriden(T& base, T* obj, R T::* func) 
{
	int index = getMethodVTableIndex(base, func);
	return (methodAddressByVTableIndex(obj, index) != methodAddressByVTableIndex(&base, index));
}