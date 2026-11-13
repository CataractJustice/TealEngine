#pragma once
#include <cstddef>  // for std::size_t
#include <utility>  // for std::move

template <typename T, std::size_t N = 0>
class StaticSizeArray {
private:
	T* data_;
	std::size_t size_;

public:
	// Default constructor
	StaticSizeArray() : data_(nullptr), size_(N) {
		if (N > 0) {
			data_ = new T[N](); // default-initialize N elements
		}
	}

	// Constructor with custom size
	StaticSizeArray(std::size_t size) : data_(nullptr), size_(size) {
		if (size_ > 0) {
			data_ = new T[size_]();
		}
	}

	// Deleted copy constructor and assignment to prevent accidental copies
	StaticSizeArray(const StaticSizeArray&) = delete;
	StaticSizeArray& operator=(const StaticSizeArray&) = delete;

	// Move constructor
	StaticSizeArray(StaticSizeArray&& other) noexcept
		: data_(other.data_), size_(other.size_) {
		other.data_ = nullptr;
		other.size_ = 0;
	}

	// Move assignment
	StaticSizeArray& operator=(StaticSizeArray&& other) noexcept {
		if (this != &other) {
			delete[] data_;
			data_ = other.data_;
			size_ = other.size_;
			other.data_ = nullptr;
			other.size_ = 0;
		}
		return *this;
	}

	// Destructor
	~StaticSizeArray() {
		delete[] data_;
	}

	// Optional init function if array was not allocated at construction
	void init(std::size_t size) {
		if (data_) delete[] data_;
		size_ = size;
		data_ = new T[size_]();
	}

	// Access operator[]
	T& operator[](std::size_t index) {
		return data_[index];
	}

	const T& operator[](std::size_t index) const {
		return data_[index];
	}
	T* data() 
	{
		return data_;
	}
	// Return size
	std::size_t size() const { return size_; }

	// Iterator support
	T* begin() { return data_; }
	T* end() { return data_ + size_; }
	
	const T* begin() const { return data_; }
	const T* end() const { return data_ + size_; }
	
	const T* cbegin() const { return data_; }
	const T* cend() const { return data_ + size_; }
};