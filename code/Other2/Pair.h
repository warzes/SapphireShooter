#pragma once

#include "Hash.h"

// Pair template class.
<<<<<<< HEAD
template <class T, class U> class Pair
=======
template <class T, class U> 
class Pair
>>>>>>> caaf2bd02a14c6a51dfcdbd73e34fff7259f3bc5
{
public:
	// Construct undefined.
	Pair()
	{
	}

	// Construct with values.
	Pair(const T& first_, const U& second_) :
		first(first_),
		second(second_)
	{
	}

	// Test for equality with another pair.
	bool operator==(const Pair<T, U>& rhs) const { return first == rhs.first && second == rhs.second; }
	// Test for inequality with another pair.
	bool operator!=(const Pair<T, U>& rhs) const { return !(*this == rhs); }

	// Test for less than with another pair.
	bool operator<(const Pair<T, U>& rhs) const
	{
		if (first < rhs.first)
			return true;
		if (first != rhs.first)
			return false;
		return second < rhs.second;
	}

	// Test for greater than with another pair.
	bool operator>(const Pair<T, U>& rhs) const
	{
		if (first > rhs.first)
			return true;
		if (first != rhs.first)
			return false;
		return second > rhs.second;
	}

	// Return hash value for HashSet & HashMap.
	unsigned ToHash() const { return (MakeHash(first) & 0xffff) | (MakeHash(second) << 16); }

	// First value.
	T first;
	// Second value.
	U second;
};

// Construct a pair.
template <class T, class U> Pair<T, U> MakePair(const T& first, const U& second)
{
	return Pair<T, U>(first, second);
}