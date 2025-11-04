#pragma once
#include<vector>
#include<functional>
#include<stdexcept>
#include <cmath>
#include <fstream>  

class RowOffsets
{
	size_t row;
	std::vector<size_t> offsets;
	void EnsureOffsets(size_t row)
	{
		while (offsets.size() <= row)
		{
			if (offsets.empty())
			{
				offsets.push_back(0);
			}
			else
			{
				offsets.push_back(offsets.back() + offsets.size());
			}
		}
	}
public:
	size_t GetOffset(size_t row)
	{
		EnsureOffsets(row);
		return offsets[row];
	}
};


extern RowOffsets counter;

template<class T>
class TriangleMatrix
{
	T* matrix;
	size_t size;

public:
	TriangleMatrix();
	TriangleMatrix(size_t size);
	TriangleMatrix(TriangleMatrix& copy);
	TriangleMatrix(TriangleMatrix&& copy);
	~TriangleMatrix();

	T Get(size_t i, size_t j) const;
	void Set(T value, size_t i, size_t j);

	TriangleMatrix operator+(TriangleMatrix& mx);
	TriangleMatrix operator-(TriangleMatrix& mx);
	TriangleMatrix operator*(TriangleMatrix& mx);
	TriangleMatrix operator*(T& mx);
	TriangleMatrix operator/(T& mx);
	TriangleMatrix& operator=(TriangleMatrix& mx);
	bool operator==(TriangleMatrix& mx);
	bool operator!=(TriangleMatrix& mx);

	class RowAccessor
	{
		size_t cols;
		T* data;
		T& operator[](size_t cols)
		{
			return data[cols];
		}
	};

	RowAccessor operator[](size_t row)
	{
	if (row >= size) 
	{
        throw std::out_of_range("Row index out of range");
    }
    return RowAccessor(row, matrix + counter.GetOffset(row));  
	}

	void SaveToFile(const std::string& filename) const;
	void LoadFromFile(const std::string& filename);

	template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const TriangleMatrix<U>& mx);

    template<typename U>
    friend std::istream& operator>>(std::istream& is, TriangleMatrix<U>& mx);

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

	Iterator begin() const;
	Iterator end() const;


	void ApplyLambda(std::function<void(T&)> lambda);

	int CountOccurrences(const T& value) const;
	std::vector<std::pair<size_t, size_t>> FindAll(const T& value) const;
	TriangleMatrix Power(int exponent) const;
	T FirstNorm() const;
	double SecondNorm() const;
	double HelderNorm(double p) const;
	T InfinityNorm() const;
};

template<class T>
inline TriangleMatrix<T>::TriangleMatrix() : size(0), matrix(nullptr)
{}

template<class T>
inline TriangleMatrix<T>::TriangleMatrix(size_t size) : size(size), matrix(new T[counter.GetOffset(size)])
{}

template<class T>
inline TriangleMatrix<T>::TriangleMatrix(TriangleMatrix& copy)
{
	delete[] matrix;
	size = copy.size;
	matrix = new T[counter.GetOffset(size)];
	for (int i = 0; i < counter.GetOffset(size); i++)
	{
		matrix[i] = copy.matrix[i];
	}
}

template<class T>
inline TriangleMatrix<T>::TriangleMatrix(TriangleMatrix&& copy)
{
	matrix = copy.matrix;
	size = copy.size;
	copy.matrix = nullptr;
	copy.size = 0;
}

template<class T>
inline TriangleMatrix<T>::~TriangleMatrix()
{
	delete[] matrix;
}

template<class T>
inline T TriangleMatrix<T>::Get(size_t i, size_t j) const
{
	if (i >= size || j >= size)
	{
		throw std::out_of_range("Matrix indices out of range");
	}
	if (j > i)
	{
		return T();
	}
	size_t offset = counter.GetOffset(i);
	return matrix[offset + j];	
}

template<class T>
inline void TriangleMatrix<T>::Set(T value, size_t i, size_t j)
{
	if (i >= size || j >= size)
	{
		throw std::out_of_range("Matrix indices out of range");
	}
	if (j > i)
	{
		throw std::invalid_argument("trying to set element above main diagonal");
	}
	size_t offset = counter.GetOffset(i);
	matrix[offset + j] = value;
}

template<class T>
inline TriangleMatrix<T> TriangleMatrix<T>::operator+(TriangleMatrix& mx)
{
	
	if (size != mx.size)
	{
		throw std::invalid_argument("dimensions aren't equal");
	}
	TriangleMatrix res(size);
	for (int i = 0; i < counter.GetOffset(size); i++)
	{
		res.matrix[i] = matrix[i] + mx.matrix[i];
	}
	return res;
}

template<class T>
inline TriangleMatrix<T> TriangleMatrix<T>::operator-(TriangleMatrix& mx)
{
	if (size != mx.size)
	{
		throw std::invalid_argument("dimensions aren't equal");
	}
	TriangleMatrix res(size);
	for (int i = 0; i < counter.GetOffset(size); i++)
	{
		res.matrix[i] = matrix[i] - mx.matrix[i];
	}
	return res;
}

template<class T>
inline TriangleMatrix<T> TriangleMatrix<T>::operator*(TriangleMatrix& mx)
{
	TriangleMatrix res(size);
	for (size_t i = 0; i < size; i++) //строчки
	{
		for (size_t j = 0; j < i; j++) //столбцы
		{
			T value;
			for (size_t k = 0; k < i; k++)
			{
				value += Get(i, k) * Get(k, j);
			}
			res.Set(i, j, value);
		}
	}
}




template<class T>
inline TriangleMatrix<T> TriangleMatrix<T>::operator*(T& mx)
{
	TriangleMatrix result(size);
	for (int i = 0; i < counter.GetOffset(size); ++i)
	{
		result.matrix[i] = matrix[i] * mx;
	}
	return result;
}

template<class T>
inline TriangleMatrix<T> TriangleMatrix<T>::operator/(T& mx)
{
	if (mx == T(0))
	{
		throw std::invalid_argument("Division by zero");
	}
	TriangleMatrix result(size);
	for (int i = 0; i < counter.GetOffset(size); ++i)
	{
		result.matrix[i] = matrix[i] / mx;
	}
	return result;
}

template<class T>
inline TriangleMatrix<T>& TriangleMatrix<T>::operator=(TriangleMatrix& mx)
{
	if (this != &mx)
	{
		delete[] matrix;
		size = mx.size;
		matrix = new T[counter.GetOffset(size)];
		for (int i = 0; i < counter.GetOffset(size); ++i)
		{
			matrix[i] = mx.matrix[i];
		}
	}
	return (*this);
}

template<class T>
inline bool TriangleMatrix<T>::operator==(TriangleMatrix& mx)
{
	if (size != mx.size)
	{
		return false;
	}
	for (int i = 0; i < counter.GetOffset(size); ++i)
	{
		if (matrix[i] != mx.matrix[i])
		{
			return false;
		}
	}
	return true;
}

template<class T>
inline bool TriangleMatrix<T>::operator!=(TriangleMatrix& mx)
{
	return !(*this == mx);
}

template<class T>
inline TriangleMatrix<T>::Iterator::Iterator(T* p) : ptr(p)
{}

template<class T>
inline typename TriangleMatrix<T>::Iterator& TriangleMatrix<T>::Iterator::operator++()
{
	if (ptr) 
	{
		++ptr;
	}
	return *this;
}

template<class T>
inline T& TriangleMatrix<T>::Iterator::operator*() const
{
	if (!ptr) 
	{
		throw std::runtime_error("Dereferencing null iterator");
	}
	return *ptr;
}

template<class T>
inline T* TriangleMatrix<T>::Iterator::operator->() const
{
	if (!ptr) 
	{
		throw std::runtime_error("Accessing through null iterator");
	}
	return ptr;
}

template<class T>
inline bool TriangleMatrix<T>::Iterator::operator==(const Iterator& other) const
{
	return ptr == other.ptr;
}

template<class T>
inline bool TriangleMatrix<T>::Iterator::operator!=(const Iterator& other) const
{
	return ptr != other.ptr;
}

template<class T>
typename TriangleMatrix<T>::Iterator TriangleMatrix<T>::begin() const
{
	return Iterator(matrix);
}

template<class T>
typename TriangleMatrix<T>::Iterator TriangleMatrix<T>::end() const
{
	return Iterator(matrix+size);
}

template<class T>
inline void TriangleMatrix<T>::ApplyLambda(std::function<void(T&)> lambda)
{
	for (auto ptr = begin(); ptr != end(); ++ptr) 
	{
		lambda(*ptr);
	}
}

template<class T>
inline void TriangleMatrix<T>::SaveToFile(const std::string& filename) const
{
	std::ofstream file(filename);
	if (!file.is_open()) 
	{
		throw std::runtime_error("Cannot open file: " + filename);
	}

	
	file << size << "\n";

	
	for (size_t i = 0; i < size; i++) 
	{
		for (size_t j = 0; j <= i; j++) 
		{ 
			file << Get(i, j) << " ";
		}
		file << "\n";
	}

	file.close();
}

template<class T>
inline void TriangleMatrix<T>::LoadFromFile(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) 
	{
		throw std::runtime_error("Cannot open file: " + filename);
	}

	
	size_t new_size;
	file >> new_size;

	
	TriangleMatrix temp(new_size);

	
	for (size_t i = 0; i < new_size; i++) 
	{
		for (size_t j = 0; j <= i; j++) 
		{
			T value;
			file >> value;
			temp.Set(i, j, value);
		}
	}

	
	*this = std::move(temp);
	file.close();
}

template<class T>
std::ostream& operator<<(std::ostream& os, const TriangleMatrix<T>& mx)
{
	
	for (size_t i = 0; i < mx.size; i++) 
	{
		for (size_t j = 0; j < mx.size; j++) 
		{
			os << mx.Get(i, j) << " ";
		}
		os << "\n";
	}
	return os;
}

template<class T>
std::istream& operator>>(std::istream& is, TriangleMatrix<T>& mx)
{
	size_t n;
	is >> n;

	TriangleMatrix<T> temp(n);

	
	for (size_t i = 0; i < n; i++) 
	{
		for (size_t j = 0; j < n; j++) 
		{
			T value;
			is >> value;
			if (j <= i) 
			{ //  только нижний 
				temp.Set(i, j, value);
			}
		}
	}

	mx = std::move(temp);
	return is;
}

template<class T>
inline int TriangleMatrix<T>::CountOccurrences(const T& value) const
{
	int count = 0;
	for (auto it = begin(); it != end(); ++it) 
	{
		if (*it == value) 
		{
			++count;
		}
	}
	return count;
}

template<class T>
inline std::vector<std::pair<size_t, size_t>> TriangleMatrix<T>::FindAll(const T& value) const
{
	std::vector<std::pair<size_t, size_t>> positions;
	size_t index = 0;
	for (auto it = begin(); it != end(); ++it, ++index) 
	{
		if (*it == value) {
			size_t i = 0;
			size_t cumulative = 0;
			while (i < size && cumulative + (i + 1) <= index) 
			{
				cumulative += (i + 1);
				i++;
			}
			size_t j = index - cumulative;
			positions.push_back({ i, j });
		}
	}
	return positions;
}

template<class T>
inline TriangleMatrix<T> TriangleMatrix<T>::Power(int exponent) const
{
	if (exponent < 0) {
		throw std::invalid_argument("Exponent can't be negative");
	}

	if (exponent == 0) {
		TriangleMatrix<T> result(size);
		for (size_t i = 0; i < size; i++) 
		{
			result.Set(i, i, T(1));
		}
		return result;
	}

	if (exponent == 1) 
	{
		return TriangleMatrix<T>(*this);
	}

	TriangleMatrix<T> result = *this;
	TriangleMatrix<T> buf = *this;
	int n = exponent - 1;

	while (n > 0) {
		if (n % 2 == 1) 
		{
			result = result * buf;
			n = n - 1;
		}
		else {
			buf = buf * buf;
			n = n / 2;
		}
	}

	return result;
}

template<class T>
inline T TriangleMatrix<T>::FirstNorm() const
{
	if (size == 0) 
	{
		return T(0);
	}

	T maxSum = T(0);
	for (size_t j = 0; j < size; j++) 
	{
		T sum = T(0);
		for (size_t i = j; i < size; i++) 
		{ 
			sum += std::abs(Get(i, j));
		}
		if (sum > maxSum) 
		{
			maxSum = sum;
		}
	}
	return maxSum;
}

template<class T>
inline double TriangleMatrix<T>::SecondNorm() const
{
	double sum = 0.0;
	for (auto it = begin(); it != end(); ++it) 
	{
		double value = static_cast<double>(*it);
		sum += value * value;
	}
	return std::sqrt(sum);
}

template<class T>
inline double TriangleMatrix<T>::HelderNorm(double p) const
{
	if (p < 1.0) {
		throw std::invalid_argument("p must be >= 1.0");
	}

	double sum = 0.0;
	for (auto it = begin(); it != end(); ++it) {
		sum += std::pow(std::abs(static_cast<double>(*it)), p);
	}
	return std::pow(sum, 1.0 / p);
}

template<class T>
inline T TriangleMatrix<T>::InfinityNorm() const
{
	if (size == 0) 
	{
		return T(0);
	}

	T maxSum = T(0);
	for (size_t i = 0; i < size; i++) 
	{
		T rowSum = T(0);
		for (size_t j = 0; j <= i; j++) 
		{ 
			rowSum += std::abs(Get(i, j));
		}
		if (rowSum > maxSum) 
		{
			maxSum = rowSum;
		}
	}
	return maxSum;
}
