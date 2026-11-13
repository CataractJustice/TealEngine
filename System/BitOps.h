template <class T>
void setBitOfArray(T* array, int bitIndex, bool bit) 
{
	const int elementSize = sizeof(T) * 8;
	if(bit)
	{
		array[bitIndex / elementSize] |= 1ULL << (bitIndex % elementSize);
		return;
	}
	array[bitIndex / elementSize] &= ~(1ULL << (bitIndex % elementSize));	 
}
template <class T>
bool getBitOfArray(T* array, int bitIndex) 
{
	const int elementSize = sizeof(T) * 8;
	return array[bitIndex / elementSize] & (1ULL << (bitIndex % elementSize));	 
}