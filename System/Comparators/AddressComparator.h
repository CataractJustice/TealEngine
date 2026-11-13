#pragma once
namespace TealEngine
{
	class AddressComparator 
	{
	public:
		template <class T>
		bool operator()(const T* left, const T* right) const
		{
			return (long long)(left) < (long long)(right);
		}
	};
}