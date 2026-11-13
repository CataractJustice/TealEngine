#include "BlockAllocator.h"
#include <exception>
#include <stdexcept>
namespace TealEngine 
{
	const unsigned long long  byteSizeInBits = 8;
	const unsigned long long bitsPerMaskUnit = sizeof(unsigned long long) * byteSizeInBits;
	char* BlockAllocator::alloc() 
	{
		if(freeStackSize) 
		{
			const unsigned long long index = freeStack[--freeStackSize];
			const unsigned long long maskIndex = index / bitsPerMaskUnit;
			const unsigned long long maskBitIndex = (index % bitsPerMaskUnit);
			if(allocationMask[maskIndex] & (1ULL << maskBitIndex)) 
			{
				throw std::logic_error("BlockAllocator: double allocation");
			}
			allocationMask[maskIndex] |= (1ULL << maskBitIndex);
			return &memoryBlock[index * blockSize];
		}
		else 
		{
			throw std::overflow_error("BlockAllocator: ran out of memory");
		}
		return nullptr;
	}
	void BlockAllocator::free(char* ptr) 
	{
		const unsigned long long relativePtr = (ptr - memoryBlock);
		const unsigned long long index = relativePtr / blockSize;
		if((relativePtr % blockSize) || index >= blockCount) 
		{
			throw std::invalid_argument("BlockAllocator: pointer is out of range or misaligned");
		}
		const unsigned long long maskIndex = index / bitsPerMaskUnit;
		const unsigned long long maskBitIndex = (index % bitsPerMaskUnit);
		if(!(allocationMask[maskIndex] & (1ULL << maskBitIndex))) 
		{
			throw std::invalid_argument("BlockAllocator: pointer is not yet allocated by this allocator, possible double free");
		}
		allocationMask[maskIndex] &= ~(1ULL << maskBitIndex);
		if((freeStackSize + 1) > blockCount) 
		{
			throw std::runtime_error("BlockAllocator: free stack overflow");
		}
		freeStack[freeStackSize] = index;
		freeStackSize++;
		
	}
	BlockAllocator::BlockAllocator(unsigned long long blockSize, unsigned long long blockCount) : 
	blockSize(blockSize), 
	blockCount(blockCount),
	memoryBlock(new char[blockSize * blockCount]),
	freeStack(new unsigned long long[blockCount]),
	freeStackSize(blockCount)
	{
		for(int i = 0; i < blockCount; i++) 
		{
			freeStack[i] = blockCount - i - 1;
		}
		const bool sizeAligned = (blockCount % bitsPerMaskUnit) == 0;
		const unsigned long long maskElementsCount = (blockCount / bitsPerMaskUnit) + (sizeAligned ? 0 : 1);
		allocationMask = new unsigned long long[maskElementsCount]();
	}
	BlockAllocator::~BlockAllocator() 
	{
		delete[] memoryBlock;
		delete[] freeStack;
		delete[] allocationMask;
	}
}