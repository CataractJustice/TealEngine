
namespace TealEngine 
{
	class BlockAllocator 
	{
		private:
		char* memoryBlock;
		unsigned long long* freeStack;
		unsigned long long* allocationMask;
		unsigned long long freeStackSize;
		unsigned long long blockSize;
		unsigned long long blockCount;

		public:
		char* alloc();
		void free(char*);
		BlockAllocator(unsigned long long blockSize, unsigned long long blockCount);
		~BlockAllocator();
	};
}