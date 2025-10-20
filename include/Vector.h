#pragma once
#include<exception>
#include<functional>
#include<string>
#include <fstream>
#include <iostream>
#include<cmath>

template <typename T>
class TVector 
{
protected:
	int sz;
	int capacity;
	T* v;
public:
	TVector();
	TVector(int size);
	TVector(int capacity, int size, T* Vec);
	TVector(TVector& copy);
	TVector(TVector&& copy);
	~TVector();

	int GetCapacity();
	int GetSize();
	T* GetVector();
	void SetSize(int size);
	void SetVector(T* vector);

	TVector<T>& operator+(const TVector& other);
	TVector<T>&  operator-(const TVector<T>& other);
	TVector<T>& operator*(const double mul);
	TVector<T>& operator/(const double div);
	T operator*(const TVector<T>& other);
	TVector<T>& operator=(const TVector<T>& other);
	bool operator==(const TVector<T>& other);
	bool operator!=(const TVector<T>& other);
	T& operator[](int index);

	
	class Iterator
	{
	private:
		T* ptr;
	public:
		Iterator(T* p = nullptr);
		Iterator& operator++();
		T& operator*() const;
		T* operator->() const;
		bool operator==(const Iterator& other) const;
		bool operator!=(const Iterator& other) const;
	};

	Iterator begin();
	Iterator end();

	void Resize(int size);
	void PushFront(const T& value);
	void PushBack(const T& value);
	void ApplyLambda(std::function<void(T&)> lambda);

	
	void SaveToFile(const std::string& filename) const;
	void LoadFromFile(const std::string& filename);

	friend std::ostream& operator<<(std::ostream& os, const TVector& vec);
	friend std::istream& operator>>(std::istream& is, TVector& vec);
};



template <typename T>
TVector<T>::TVector() : v(nullptr), sz(0), capacity(0)
{
};

template <typename T>
TVector<T>::TVector(int size)
{
	capacity = size+log2(size);
	sz = size;
	v = new T[capacity];
};

template <typename T>
TVector<T>::TVector(int capacity_, int size_, T* Vec_)
{
	capacity = capacity_;
	sz = size_;
	v = Vec_;
};

template <typename T>
TVector<T>::TVector(TVector& other)
{
	capacity = other.capacity;
	sz = other.sz;
	v = new T[capacity];
	for (int i = 0; i < sz; i++)
	{
		v[i] = other.v[i];
	}
};

template <typename T>
TVector<T>::TVector(TVector&& other)
{
	sz = other.size;
	capacity = other.capacity;
	v = other.v;
	other.v = nullptr;
	other.sz = 0;
	other.capacity = 0;
}
template<typename T>
TVector<T>::~TVector()
{
	delete[] v;
}
template<typename T>
int TVector<T>::GetSize()
{
	return sz;
}
template<typename T>
int TVector<T>::GetCapacity()
{
	return capacity;
}


template<typename T>
T* TVector<T>::GetVector()
{
	return v;
}
template<typename T>
void TVector<T>::SetSize(int size)
{
	sz = size;
}
template<typename T>
void TVector<T>::SetVector(T* vector)
{
	v = vector;
}
template<typename T>
TVector<T>& TVector<T>::operator+(const TVector& other)
{
	if (sz == other.sz)
	{
		TVector<T> result(sz);
		for (int i = 0; i < result.sz; i++)
		{
			result.v[i] = v[i] + other.v[i];
		}
		return result;
	}
	else
	{
		throw std::invalid_argument("different dimensions");
	}
}
template<typename T>
TVector<T>& TVector<T>::operator-(const TVector<T>& other)
{
	if (sz == other.sz)
	{
		TVector<T> result(sz);
		for (int i = 0; i < result.sz; i++)
		{
			result.v[i] = v[i] - other.v[i];
		}
		return result;
	}
	else
	{
		throw std::invalid_argument("different dimensions");
	}
}
template<typename T>
TVector<T>& TVector<T>::operator*(const double mul)
{
	TVector<T> result(sz);
	int size = GetSize();
	for (int i = 0; i < size; i++)
	{
		result.v[i] = v[i] * mul;
	}
	return result;
}
template<typename T>
TVector<T>& TVector<T>::operator/(const double div)
{
	TVector<T> result(sz);
	int size = GetSize();
	for (int i = 0; i < size; i++)
	{
		result.v[i] = v[i] / div;
	}
	return result;
}
template<typename T>
T TVector<T>::operator*(const TVector<T>& other)
{
	if (sz == other.sz)
	{
		T result;
		int size = GetSize();
		for (int i = 0; i < size; i++)
		{
			result += v[i] * other.v[i];
		}
		return result;
	}
	else
	{
		throw std::invalid_argument("different dimensions");
	}
}

template<typename T>
TVector<T>& TVector<T>::operator=(const TVector<T>& other)
{
	if (this != &other)
	{
		delete[] v;
		sz = other.sz;
		capacity = other.capacity;
		v = new T[capacity];
		for (int i = 0; i < sz; i++)
		{
			v[i] = other.v[i];
		}
	}
	return *this;
}

template<typename T>
bool TVector<T>::operator==(const TVector<T>& other)
{
	if (sz != other.sz || capacity != other.capacity)
	{
		return false;
	}
	else
	{
		for (int i = 0; i < sz; i++) 
		{
			if (v[i] != other.v[i])
				return false;
		}
		return true;
	}
}

template<typename T>
bool TVector<T>::operator!=(const TVector<T>& other)
{
	return !(*this == other);
}

template<typename T>
T& TVector<T>::operator[](int index)
{
	if (index < 0 || index >= sz) 
	{
		throw std::out_of_range("Index out of range");
	}
	return v[index]; 
}



template<typename T>
inline TVector<T>::Iterator::Iterator(T* p)
{
	ptr = p;
}

template<typename T>
typename TVector<T>::Iterator& TVector<T>::Iterator::operator++() //dependent type
{
	if (ptr)
	{
		++ptr;
	}
	return *this;
}

template<typename T>
inline T& TVector<T>::Iterator::operator*() const
{
	if (ptr)
	{
		return *ptr;
	}
	else
	{
		throw std::runtime_error("reference to nullptr");
	}
}

template<typename T>
inline T* TVector<T>::Iterator::operator->() const
{
	if (ptr) 
	{
		return ptr;
	}
	throw std::runtime_error("Accessing through null iterator");
}

template<typename T>
inline bool TVector<T>::Iterator::operator==(const Iterator& other) const
{
	return ptr == other.ptr;
}

template<typename T>
inline bool TVector<T>::Iterator::operator!=(const Iterator& other) const
{
	return ptr != other.ptr;
}

template<typename T>
typename TVector<T>::Iterator TVector<T>::begin()
{
	return Iterator(v);
}

template<typename T>
typename TVector<T>::Iterator TVector<T>::end()
{
	return Iterator(v + sz);
}

template<typename T>
inline void TVector<T>::Resize(int size)
{
	TVector res(size);
	for (int i = 0; i < sz; i++)
	{
		res.v[i] = v[i];
	};
	(*this) = res;
}

template<typename T>
inline void TVector<T>::PushFront(const T& value)
{
	if (sz < capacity)
	{
		for (int i = sz; i > 0; i--)
		{
			v[i] = v[i - 1];
		}
		v[0] = value;
		sz = sz + 1;
	}
	else
	{
		Resize(sz + 100);
		for (int i = size; i > 0; i--)
		{
			v[size] = v[size - 1];
		}
		v[0] = value;
		size = size + 1;
		
	}
}

template<typename T>
inline void TVector<T>::PushBack(const T& value)
{
	if (size < capacity)
	{
		v[sz] = value;
		sz = sz + 1;
	}
	else
	{
		Resize(sz + 100);
		v[size] = value;
		sz = sz + 1;
	}
}

template<typename T>
inline void TVector<T>::ApplyLambda(std::function<void(T&)> lambda)
{
	for (int i = 0; i < sz; ++i) 
	{
		lambda(v[i]);
	}
}


template<typename T>
inline void TVector<T>::SaveToFile(const std::string& filename) const
{
	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open())
	{
		throw std::runtime_error("Cannot open file for writing: " + filename);
	}

	
	file.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
	file.write(reinterpret_cast<const char*>(&capacity), sizeof(capacity));

	if (sz > 0) {
		file.write(reinterpret_cast<const char*>(v), sz * sizeof(T));
	}

	file.close();
}

template<typename T>
inline void TVector<T>::LoadFromFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open())
	{
		throw std::runtime_error("Cannot open file for reading: " + filename);
	}

	
	int newSize, newCapacity;
	file.read(reinterpret_cast<char*>(&newSize), sizeof(newSize));
	file.read(reinterpret_cast<char*>(&newCapacity), sizeof(newCapacity));

	if (newSize < 0 || newCapacity < 0)
	{
		throw std::runtime_error("Invalid file format: negative size or capacity");
	}

	if (newSize > newCapacity)
	{
		throw std::runtime_error("Invalid file format: size cannot be greater than capacity");
	}

	
	delete[] v;

	
	sz = newSize;
	capacity = newCapacity;

	if (capacity > 0)
	{
		v = new T[capacity];
		if (sz > 0)
		{
			file.read(reinterpret_cast<char*>(v), sz * sizeof(T));
		}
	}
	else
	{
		v = nullptr;
	}

	file.close();
}


template<typename T>
std::ostream& operator<<(std::ostream& os, const TVector<T>& vec)
{
	os << "[";
	for (int i = 0; i < vec.sz; ++i) 
	{
		os << vec.v[i];
		if (i < vec.sz - 1) 
		{
			os << ", ";
		}
	}
	os << "]";
	return os;
}

template<typename T>
std::istream& operator>>(std::istream& is, TVector<T>& vec)
{
	delete[] vec.v;
	vec.sz = 0;
	vec.capacity = 0;
	vec.v = nullptr;

	
	int size;
	std::cout << "Enter vector size: ";
	is >> size;

	if (size <= 0) 
	{
		throw std::invalid_argument("Size must be positive");
	}

	
	vec.sz = size;
	vec.capacity = size + static_cast<int>(log2(size));
	vec.v = new T[vec.capacity];

	
	std::cout << "Enter " << size << " elements: ";
	for (int i = 0; i < size; ++i) 
	{
		is >> vec.v[i];
	}

	return is;
}

