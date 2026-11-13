#include <vector>
template<class T>
class CompactStableIndexContainer 
{
	std::vector<int> stableIndexToRealIndexMapping;
	std::vector<int> realIndexToStableIndexMapping;
	std::vector<T> elements;
	CompactStableIndexContainer(unsigned int mappingSize) : 
	stableIndexToRealIndexMapping(), 
	realIndexToStableIndexMapping(),
	elements()
	{
		stableIndexToRealIndexMapping.resize(mappingSize);
		for(int& i : stableIndexToRealIndexMapping) 
		{
			i = -1;
		}
	}
	
}