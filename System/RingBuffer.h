template<class T>
class RingBufferQueue 
{
	T* buffer;
	int capacity;
	int sizeValue;
	int frontIndex;
	int backIndex;
	public:
	RingBufferQueue(int capacity) : frontIndex(0), backIndex(0), sizeValue(0), capacity(capacity) { buffer = new T[capacity]; }
	~RingBufferQueue() { delete[] buffer; }
	inline T& front() { return buffer[frontIndex]; }
	inline void pop() { frontIndex = (frontIndex + 1) % capacity; sizeValue--; }
	inline void push(const T& val) { buffer[backIndex++] = val; backIndex %= capacity; sizeValue++; }
	inline int size() { return sizeValue; }
	RingBufferQueue(const RingBufferQueue&) = delete;
	RingBufferQueue& operator=(const RingBufferQueue&) = delete;
};