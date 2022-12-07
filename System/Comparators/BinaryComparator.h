#pragma once

namespace TealEngine 
{
	class BinaryComparator 
	{
	public:
		template <class T>
		bool operator()(const T left, const T right) const
		{
			unsigned int size = sizeof(T);
			for (int i = 0; i < size; i++) 
			{
				if (((char*)&left)[i] == ((char*)&right)[i])
					continue;
				return ((char*)&left)[i] < ((char*)&right)[i];
			}
			return false;
		}
	};
}