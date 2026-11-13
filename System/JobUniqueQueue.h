#include "MutexDataPair.h"
#include <thread>
#include <unordered_map>
#include <set>
#include <map>

/*
Create job
Get job
Release job ? - 
Remove job
*/

//typedef int T;
//typedef std::less<T> Comparator;
template<typename T, typename Comparator = std::less<T>>
class JobUniqueQueue 
{
	private:

	std::vector<T> ringBuffer;
	std::set<T, Comparator> jobSet; 
	int head;
	int tail;

	std::mutex mut;

	public:
	JobUniqueQueue() 
	{
		head = 0;
		tail = 0;
		ringBuffer.resize(4096);
	}
	int jobCount() 
	{
		std::lock_guard<std::mutex> lock(mut);
		return ((head - tail) + ringBuffer.size()) % ringBuffer.size();
	}
	void createJob(const T& val) 
	{
		std::lock_guard<std::mutex> lock(mut);
		if(jobSet.emplace(val).second == false) return; 
		ringBuffer[head++] = val;
		head %= ringBuffer.size();
	}
	bool tryGetJob(T& val) 
	{
		std::lock_guard<std::mutex> lock(mut);
		if(head == tail) 
		{
			return false;
		}
		val = ringBuffer[tail++];
		tail %= ringBuffer.size();
		jobSet.erase(val);
		return true;
	}
};