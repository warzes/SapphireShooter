#pragma once

// Random access iterator.
template <class T> 
struct RandomAccessIterator
{
	// Construct a null iterator.
	constexpr RandomAccessIterator() = default;
	// Construct with an object pointer.
	constexpr explicit RandomAccessIterator(T* ptr_) noexcept : ptr(ptr_) {}

	// Point to the object.
	constexpr T* operator->() const noexcept { return ptr; }
	// Dereference the object.
	constexpr T& operator*() const noexcept { return *ptr; }

	// Preincrement the pointer.
	constexpr RandomAccessIterator<T>& operator++() noexcept { ++ptr; return *this; }
	// Postincrement the pointer.
	constexpr RandomAccessIterator<T> operator++(int) noexcept { RandomAccessIterator<T> i = *this; ++ptr; return i; }
	// Predecrement the pointer.
	constexpr RandomAccessIterator<T>& operator--() noexcept { --ptr; return *this; }
	// Postdecrement the pointer.
	constexpr RandomAccessIterator<T> operator--(int) noexcept { RandomAccessIterator<T> i = *this; --ptr; return i; }
	// Add an offset to the pointer.
	constexpr RandomAccessIterator<T>& operator+=(int value) noexcept { ptr += value; return *this; }
	constexpr RandomAccessIterator<T>& operator+=(size_t value) noexcept { ptr += value; return *this; }
	// Subtract an offset from the pointer.
	constexpr RandomAccessIterator<T>& operator-=(int value) noexcept { ptr -= value; return *this; }
	constexpr RandomAccessIterator<T>& operator-=(size_t value) noexcept { ptr -= value; return *this; }
	// Add an offset to the pointer.
	constexpr RandomAccessIterator<T> operator+(int value) const noexcept { return RandomAccessIterator<T>(ptr + value); }
	constexpr RandomAccessIterator<T> operator+(size_t value) const noexcept { return RandomAccessIterator<T>(ptr + value); }
	// Subtract an offset from the pointer.
	constexpr RandomAccessIterator<T> operator-(int value) const noexcept { return RandomAccessIterator<T>(ptr - value); }
	constexpr RandomAccessIterator<T> operator-(size_t value) const noexcept { return RandomAccessIterator<T>(ptr - value); }
	// Calculate offset to another iterator.
	constexpr int operator-(const RandomAccessIterator& rhs) const noexcept { return (int)(ptr - rhs.ptr); }
	
	// Test for equality with another iterator.
	constexpr bool operator==(const RandomAccessIterator& rhs) const noexcept { return ptr == rhs.ptr; }
	// Test for inequality with another iterator.
	constexpr bool operator!=(const RandomAccessIterator& rhs) const noexcept { return ptr != rhs.ptr; }
	// Test for less than with another iterator.
	constexpr bool operator<(const RandomAccessIterator& rhs) const noexcept { return ptr < rhs.ptr; }
	// Test for greater than with another iterator.
	constexpr bool operator>(const RandomAccessIterator& rhs) const noexcept { return ptr > rhs.ptr; }
	// Test for less than or equal with another iterator.
	constexpr bool operator<=(const RandomAccessIterator& rhs) const noexcept { return ptr <= rhs.ptr; }
	// Test for greater than or equal with another iterator.
	constexpr bool operator>=(const RandomAccessIterator& rhs) const noexcept { return ptr >= rhs.ptr; }

	// Pointer to the random-accessed object(s).
	T* ptr = nullptr;
};

// Random access const iterator.
template <class T> 
struct RandomAccessConstIterator
{
	// Construct a null iterator.
	constexpr RandomAccessConstIterator() noexcept : ptr(nullptr) {}
	// Construct from a raw pointer.
	constexpr explicit RandomAccessConstIterator(const T* ptr_) noexcept : ptr(ptr_) {}
	// Construct from a non-const iterator.
	constexpr RandomAccessConstIterator(const RandomAccessIterator<T>& it) noexcept : ptr(it.ptr) {}

	// Assign from a non-const iterator.
	constexpr RandomAccessConstIterator<T>& operator=(const RandomAccessIterator<T>& rhs) noexcept { ptr = rhs.ptr; return *this; }
	
	// Point to the object.
	constexpr const T* operator->() const noexcept { return ptr; }
	// Dereference the object.
	constexpr const T& operator*() const noexcept { return *ptr; }
	
	// Preincrement the pointer.
	constexpr RandomAccessConstIterator<T>& operator++() noexcept { ++ptr; return *this; }
	// Postincrement the pointer.
	constexpr RandomAccessConstIterator<T> operator++(int) noexcept { RandomAccessConstIterator<T> i = *this; ++ptr; return i; }
	// Predecrement the pointer.
	constexpr RandomAccessConstIterator<T>& operator--() noexcept { --ptr; return *this; }
	// Postdecrement the pointer.
	constexpr RandomAccessConstIterator<T> operator--(int) noexcept { RandomAccessConstIterator<T> i = *this; --ptr; return i; }
	// Add an offset to the pointer.
	constexpr RandomAccessConstIterator<T>& operator+=(int value) noexcept { ptr += value; return *this; }
	constexpr RandomAccessConstIterator<T>& operator+=(size_t value) noexcept { ptr += value; return *this; }
	// Subtract an offset from the pointer.
	constexpr RandomAccessConstIterator<T>& operator-=(int value) noexcept { ptr -= value; return *this; }
	constexpr RandomAccessConstIterator<T>& operator-=(size_t value) noexcept { ptr -= value; return *this; }
	// Add an offset to the pointer.
	constexpr RandomAccessConstIterator<T> operator+(int value) const noexcept { return RandomAccessConstIterator<T>(ptr + value); }
	constexpr RandomAccessConstIterator<T> operator+(size_t value) const noexcept { return RandomAccessConstIterator<T>(ptr + value); }
	// Subtract an offset from the pointer.
	constexpr RandomAccessConstIterator<T> operator-(int value) const noexcept { return RandomAccessConstIterator<T>(ptr - value); }
	constexpr RandomAccessConstIterator<T> operator-(size_t value) const noexcept { return RandomAccessConstIterator<T>(ptr - value); }
	// Calculate offset to another iterator.
	constexpr int operator-(const RandomAccessConstIterator& rhs) const noexcept { return (int)(ptr - rhs.ptr); }
	// Test for equality with another iterator.
	constexpr bool operator==(const RandomAccessConstIterator& rhs) const noexcept { return ptr == rhs.ptr; }
	// Test for inequality with another iterator.
	constexpr bool operator!=(const RandomAccessConstIterator& rhs) const noexcept { return ptr != rhs.ptr; }
	// Test for less than with another iterator.
	constexpr bool operator<(const RandomAccessConstIterator& rhs) const noexcept { return ptr < rhs.ptr; }
	// Test for greater than with another iterator.
	constexpr bool operator>(const RandomAccessConstIterator& rhs) const noexcept { return ptr > rhs.ptr; }
	// Test for less than or equal with another iterator.
	constexpr bool operator<=(const RandomAccessConstIterator& rhs) const noexcept { return ptr <= rhs.ptr; }
	// Test for greater than or equal with another iterator.
	constexpr bool operator>=(const RandomAccessConstIterator& rhs) const noexcept { return ptr >= rhs.ptr; }

	// Pointer to the random-accessed object(s).
	const T* ptr;
};