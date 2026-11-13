#pragma once
#include <shared_mutex>
#include <mutex>
#include <functional>
template<class T>
class MutexDataPair 
{
	private:
	mutable std::shared_mutex smut;
	public:
	T data;

 	template <typename... Args>
	explicit MutexDataPair(Args&&... args) : data(std::forward<Args>(args)...) {}

	~MutexDataPair() 
	{
	}

	class SharedLockAccess 
	{
		private:
		const T* data;
		std::shared_mutex* smut;
		bool locked;
		public:
		SharedLockAccess(const T* data, std::shared_mutex* smut) : data(data), smut(smut) 
		{
			locked = true;
			smut->lock_shared();
		}
		~SharedLockAccess() 
		{
			if(locked)
			smut->unlock_shared(); 
		}
		const T* operator ->() const
		{
			return data;
		}
		const T& operator*() const
		{
			return *data;
		}
		void unlock() 
		{
			locked = false;
			smut->unlock_shared();
		}
	};
	class LockAccess
	{
		private:
		T* data;
		std::shared_mutex* smut;
		bool locked;
		public:
		LockAccess(T* data, std::shared_mutex* smut) : data(data), smut(smut) 
		{
			locked = true;
			smut->lock();
		}
		~LockAccess() 
		{
			if(locked)
			smut->unlock(); 
		}
		T* operator ->() const
		{
			return data;
		}
		T& operator*() const
		{
			return *data;
		}
		void unlock() 
		{
			locked = false;
			smut->unlock();
		}
	};

	class TrySharedLockAccess 
	{
		private:
		const T* data;
		std::shared_mutex* smut;
		bool locked;
		public:
		inline bool isLocked() { return locked; }
		TrySharedLockAccess(const T* data, std::shared_mutex* smut) : data(data), smut(smut) 
		{
			locked = smut->try_lock_shared();
		}
		~TrySharedLockAccess() 
		{
			if(locked) smut->unlock_shared(); 
		}
		const T* operator ->() const
		{
			return data;
		}
		const T& operator*() const
		{
			return *data;
		}
		void unlock() 
		{
			locked = false;
			smut->unlock_shared();
		}
	};
	class TryLockAccess
	{
		private:
		T* data;
		std::shared_mutex* smut;
		bool locked;
		public:
		inline bool isLocked() { return locked; }
		TryLockAccess(T* data, std::shared_mutex* smut) : data(data), smut(smut) 
		{
			locked = smut->try_lock();
		}
		~TryLockAccess() 
		{
			if(locked) smut->unlock(); 
		}
		T* operator ->() const
		{
			return data;
		}
		T& operator*() const
		{
			return *data;
		}
		void unlock() 
		{
			locked = false;
			smut->unlock();
		}
	};
	
	void sharedAccessMethod(std::function<void(const T&)> readFunction) const
	{
		smut.lock_shared();
		readFunction(data);
		smut.unlock_shared();
	}
	void accessMethod(std::function<void(T&)> readWriteFunction) 
	{
		smut.lock();
		readWriteFunction(data);
		smut.unlock();
	}
	void trySharedAccessMethod(std::function<void(const T&)> readFunction) const
	{
		if(smut.try_lock_shared()) 
		{
			readFunction(data);
			smut.unlock_shared();
		}
	}
	void tryAccessMethod(std::function<void(T&)> readWriteFunction) 
	{
		if(smut.try_lock()) 
		{
			readWriteFunction(data);
			smut.unlock();
		}
	}
	LockAccess access()
	{
		return LockAccess(&data, &smut);
	};
	const SharedLockAccess sharedAccess() const
	{
		return SharedLockAccess(&data, &smut);
	}

	TryLockAccess tryAccess()
	{
		return TryLockAccess(&data, &smut);
	};
	const TrySharedLockAccess trySharedAccess() const
	{
		return TrySharedLockAccess(&data, &smut);
	}
};