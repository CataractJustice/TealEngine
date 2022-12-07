#pragma once
#include <utility>
template<typename T>
bool isPairContains(const std::pair<T, T>& p, const T& value) 
{
	return p.first == value || p.second == value;
}

template<typename T>
bool isPairContains(const std::pair<T, T>& p, const T& valuea, const T& valueb) 
{
	return p.first == valuea && p.second == valueb || p.first == valueb && p.second == valuea;
}

template<typename T>
bool isPairContains(const T& first, const T& second, const T& value)
{
	return first == value || second == value;
}

template<typename T>
bool isPairContains(const T& first, const T& second, const T& valuea, const T& valueb)
{
	return first == valuea && second == valueb || first == valueb && second == valuea;
}