#pragma once


#include <utility>
#include <unordered_map>

template <typename T>
class UPair {
public:
	T first, second;

	// Constructor: Always store in sorted order
	UPair(T a, T b) : first(std::min(a, b)), second(std::max(a, b)) {}

	// Equality operator: two pairs are equal if they have the same elements (order-independent)
	bool operator==(const UPair<T>& other) const {
		return first == other.first && second == other.second;
	}
};

// Hash function for UnorderedPair
namespace std {
	template <typename T>
	struct hash<UPair<T>> {
		size_t operator()(const UPair<T>& p) const {
			return hash<T>()(p.first) ^ hash<T>()(p.second);
		}
	};
}