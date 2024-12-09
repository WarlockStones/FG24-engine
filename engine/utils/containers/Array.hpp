#pragma once

namespace FG24 {

// (Reinventing the wheel for educational purposes)
template<typename T>
class Vector {
public:
	Vector();
	explicit Vector(int size);
	// Single argument constructurs should be marked explicit.
	// Explicit constructors does not provide implicit conversion.

	// Vector(const Vector&); // Copy constructor
	// Vector& operator=(const Vector&); // Copy assignment

	// Vector(Vector&&); // Move constructor;
	// Vector& operator=(Vector&&); // Move assingment

	~Vector();

	T& operator[](int index); // Access without bounds-checking
	const T& operator[](int index) const;

	// Returns current size, the current number of items
	std::size_t Size() const;
	std::size_t MaxCapacity() const; // Returns size + available free space

	// Growth
	// void Resize(std::size_t newSize);
	void PushBack(const T& toAdd);
	void Reserve(std::size_t newAlloc);

private:
	std::size_t sz; // Number of items
	T* items;	// Pointer to allocated the items
	std::size_t space; // Size + Free space

};

template<typename T>
explicit Vector::Vector(int size)
: sz(size), items{new T[size]}, space(size) {
	for (int i = 0; i < size; ++i) {
	  items[i] = 0;
	}
}

template<typename T>
void Vector::PushBack(const T& toAdd) {
	if (space == 0) {
		Reserve(8);
	} else if (size == space) {
		Reserve(space * 2);
	}

}

} // namespace FG24
