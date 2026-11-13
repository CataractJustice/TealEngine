template<typename T>
class JobFunnel 
{
	private:
	std::vector<T> ringBuffer;
	int head;
	int tail;

	std::mutex mut;
	
	public:
	JobFunnel()
	{
		//There will never be more than 4096 elements during the use.
		ringBuffer.resize(4096);
		head = 0;
		tail = 0;
	}
	//Consumer must catch up
	void add(const T& val) 
	{
		std::lock_guard<std::mutex> lock(mut);
		ringBuffer[head++] = val;
		head %= ringBuffer.size();
	}

	bool get(T& val) 
	{
		std::lock_guard<std::mutex> lock(mut);
		if(head == tail) return false;
		val = ringBuffer[tail++];
		tail %= ringBuffer.size();
		return true;
	}
};