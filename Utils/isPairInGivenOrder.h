#pragma once
#include <utility>

enum class SwapCondition
{
	DO_NOT_SWAP,
	SWAP_IF_IN_ORDER,
	SWAP_IF_NOT_IN_ORDER
};

template<typename T>
bool isPairInGivenOrder(const T& firsta, const T& seconda, const T& firstb, const T& secondb) 
{
	return (firsta == firstb && seconda == secondb);
}