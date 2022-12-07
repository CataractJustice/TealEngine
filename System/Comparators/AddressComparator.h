#pragma once
namespace TealEngine
{
	class AddressComparator 
	{
	public:
		template <class T>
		bool operator()(const T* left, const T* right) const
		{
			return (int)(&left) < (int)(&right);
		}
	};
}