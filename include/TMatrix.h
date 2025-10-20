#pragma once

#include<stdexcept>
#include<functional>
#include<string>
#include <fstream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include<cmath>

template <typename T>
class TMatrix
{
	T* m;
	int cols;
	int rows;

public:
	int GetCols();
	int GetRows();
	void SetCols(int n);
	void SetRows(int n);
	void SetMatrix(T* m_)
	{
		m = m_;
	};
	T* GetMatrix()
	{
		return m;
	};

	void Fill(const T& value);
	void Resize(int newRows, int newCols);

	void Set(T value, size_t i, size_t j);
	T Get(size_t i, size_t j) const;

	TMatrix();
	TMatrix(int rows_, int cols_);
	TMatrix(TMatrix& copy);
	TMatrix(TMatrix&& copy);
	~TMatrix();

	TMatrix<T> operator+(TMatrix& mx);
	TMatrix<T> operator-(TMatrix& mx);
	TMatrix<T> operator*(T& mx);
	TMatrix<T> operator/(T& mx);
	TMatrix<T> operator*(TMatrix& mx);
	bool operator==(TMatrix& mx);
	bool operator!=(TMatrix& mx);
	TMatrix<T>& operator=(TMatrix& other);
	
	/*T& operator[](size_t i)
	{
		if (r >= rows && r < 0)
		{
			throw std::logic_error("overflow")
		}
		return &m[i * cols];
	};

	struct ColAccessor
	{
		size_t cols;
		T* data;

		T* operator[](size_t c)
		{
			if (c >= cols)
			{
				throw std::invalid_argument("i > cols")
			}
			return data[c];
		}
	};

	ColAccesor operator[](size_t c)
	{
		if (c > cols)
		{
			throw std::invalid_argument("r >= rows")
		}
		return ColAccessor(cols, &m[r * cols]);
	}*/

	


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


	void ApplyLambda(std::function<void(T&)> lambda);


	void SaveToFile(const std::string& filename) const;
	void LoadFromFile(const std::string& filename);

	friend std::ostream& operator<<(std::ostream& os, const TMatrix& vec);
	friend std::istream& operator>>(std::istream& is, TMatrix& vec);

	struct Coordinates
	{
		int row_;
		int col_;
	};

	int CountOccurrenses(const T& value);//встречаемость элемента
	std::vector<Coordinates> FindAll(const T& value);//позиции элементов
	TMatrix Power(int exponent);
	T FirstNorm();
	double SecondNorm();
	double HelderNorm(double p);
	T InfinityNorm();
};


template<typename T>
inline int TMatrix<T>::GetCols()
{
	return cols;
}

template<typename T>
inline int TMatrix<T>::GetRows()
{
	return rows;
}

template<typename T>
inline void TMatrix<T>::SetCols(int n)
{
	cols = n;
}

template<typename T>
inline void TMatrix<T>::SetRows(int n)
{
	rows = n;
}

template<typename T>
inline void TMatrix<T>::Fill(const T& value)
{
	int size = rows * cols;
	for (int i = 0; i < size; ++i) 
	{
		m[i] = value;
	}
}

template<typename T>
inline void TMatrix<T>::Resize(int newRows, int newCols)
{
	if (newRows <= 0 || newCols <= 0) 
	{
		throw std::invalid_argument("Matrix dimensions must be positive");
	}

	
	if (newRows == rows && newCols == cols) 
	{
		return;
	}

	
	T* newData = new T[newRows * newCols];

	
	int minRows = std::min(rows, newRows);
	int minCols = std::min(cols, newCols);

	for (int i = 0; i < minRows; ++i) 
	{
		for (int j = 0; j < minCols; ++j) 
		{
			newData[i * newRows + j] = Get(i, j);
		}
	}

	
	for (int i = minRows; i < newRows; ++i) 
	{
		for (int j = minCols; j < newCols; ++j) 
		{
			newData[i * newRows + j] = T();
		}
	}

	
	delete[] m;
	m = newData;
	rows = newRows;
	cols = newCols;
}

template<typename T>
inline void TMatrix<T>::Set(T value, size_t i, size_t j)
{
	if (i >= rows || j >= cols) 
	{
		throw std::out_of_range("Matrix indices out of range");
	}
	m[i * cols + j] = value;  
}

template<typename T>
inline T TMatrix<T>::Get(size_t i, size_t j) const
{
	if(i >= cols || j >= rows)
		throw std::invalid_argument("access to > i or j");
	return m[i * cols + j];
}

template<typename T>
inline TMatrix<T>::TMatrix() : rows(0), cols(0), m(nullptr)
{}

template<typename T>
inline TMatrix<T>::TMatrix(int rows_, int cols_)
{
	rows = rows_;
	cols = cols_;
	m = new T[cols * rows];
}

template<typename T>
inline TMatrix<T>::TMatrix(TMatrix& copy)
{
	rows = copy.rows;
	cols = copy.cols;
	int size = cols * rows;
	m = new T[size];
	for (int i = 0; i < size; i++)
	{
		m[i] = copy.m[i];
	}
}

template<typename T>
inline TMatrix<T>::TMatrix(TMatrix&& copy)
{
	rows = copy.rows;
	cols = copy.cols;
	m = copy.m;
	copy.m = nullptr;
	copy.rows = 0;
	copy.cols = 0;
}

template<typename T>
inline TMatrix<T>::~TMatrix()
{
	delete[] m;  
}

template<typename T>
inline TMatrix<T> TMatrix<T>::operator+(TMatrix& mx)
{
	if (cols == mx.cols && rows == mx.rows)
	{
		TMatrix res(rows, cols);
		int size = cols * rows;
		for (int i = 0; i < size; i++)
		{
			res.m[i] = m[i] + mx.m[i];
		}
		return res;
	}
	else
	{
		throw std::invalid_argument("different dimensions");
	}
}

template<typename T>
inline TMatrix<T> TMatrix<T>::operator-(TMatrix& mx)
{
	if (cols == mx.cols && rows == mx.rows)
	{
		TMatrix res(rows, cols);
		int size = cols * rows;
		for (int i = 0; i < size; i++)
		{
			res.m[i] = m[i] - mx.m[i];
		}
		return res;
	}
	else
	{
		throw std::invalid_argument("different dimensions");
	}
}

template<typename T>
inline TMatrix<T> TMatrix<T>::operator*(T& mx)
{
	TMatrix res(rows, cols);
	int size = cols * rows;
	for (int i = 0; i < size; i++)
	{
		res.m[i] = m[i] * mx;
	}
	return res;
}

template<typename T>
inline TMatrix<T> TMatrix<T>::operator/(T& mx)
{
	if (mx == T(0)) 
	{
		throw std::invalid_argument("Division by zero");
	}
	TMatrix res(rows, cols);
	int size = cols * rows;
	for (int i = 0; i < size; i++)
	{
		res.m[i] = m[i] / mx;
	}
	return res;
}

template<typename T>
inline TMatrix<T> TMatrix<T>::operator*(TMatrix& mx)
{
	if (cols != mx.rows)
	{
		throw std::invalid_argument("cols != mx.rows");
	}

	TMatrix<T> res(rows, mx.cols);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < mx.cols; j++)
		{
			T buf = T();
			for (int k = 0; k < cols; k++)
			{
				buf += Get(i, k) * mx.Get(k, j);  
			}
			res.Set(buf, i, j);  
		}
	}
	return res;
}

template<typename T>
inline bool TMatrix<T>::operator==(TMatrix& mx)
{
	if (cols != mx.cols || rows != mx.rows) 
	{
		return false;
	}
	int size = cols * rows;
	for (int i = 0; i < size; i++) 
	{  
		if (m[i] != mx.m[i]) 
		{
			return false;
		}
	}
	return true;
}

template<typename T>
inline bool TMatrix<T>::operator!=(TMatrix& mx)
{
	return !(*this == mx);
	
}

template<typename T>
inline TMatrix<T>& TMatrix<T>::operator=(TMatrix& other)
{
	if (this != &other)
	{
		cols = other.cols;
		rows = other.rows;
		int size = cols * rows;
		delete[] m;
		m = new T[size];
		for (int i = 0; i < size; i++)
		{
			m[i] = other.m[i];
		}
	}
	return *this;
}

template<typename T>
inline TMatrix<T>::Iterator::Iterator(T* p) : ptr(p)
{
}

template<typename T>
inline typename TMatrix<T>::Iterator& TMatrix<T>::Iterator::operator++()
{
	if (ptr) 
	{
		++ptr;
	}
	return *this;
}

template<typename T>
inline T& TMatrix<T>::Iterator::operator*() const
{
	if (!ptr) 
	{
		throw std::runtime_error("Dereferencing null iterator");
	}
	return *ptr;
}

template<typename T>
inline T* TMatrix<T>::Iterator::operator->() const
{
	if (!ptr) 
	{
		throw std::runtime_error("Accessing through null iterator");
	}
	return ptr;
}

template<typename T>
inline bool TMatrix<T>::Iterator::operator==(const Iterator& other) const
{
	return ptr == other.ptr;
}

template<typename T>
inline bool TMatrix<T>::Iterator::operator!=(const Iterator& other) const
{
	return ptr != other.ptr;
}

template<typename T>
typename TMatrix<T>::Iterator TMatrix<T>::begin()
{
	return Iterator(m);
}

template<typename T>
typename TMatrix<T>::Iterator TMatrix<T>::end()
{
	return Iterator(m+cols*rows);
}


template<typename T>
inline void TMatrix<T>::ApplyLambda(std::function<void(T&)> lambda)
{
	for (Iterator i = begin(); i != end() ; ++i)
	{
		lambda(*i);
	}
}


template<typename T>
inline void TMatrix<T>::SaveToFile(const std::string& filename) const
{
	std::ofstream file(filename);
	if (!file.is_open()) 
	{
		throw std::runtime_error("Cannot open file for writing: " + filename);
	}

	
	file << rows << " " << cols << "\n";

	
	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < cols; j++) 
		{
			file << Get(i, j);
			if (j < cols - 1) 
			{
				file << " ";
			}
		}
		file << "\n";
	}

	file.close();
}

template<typename T>
inline void TMatrix<T>::LoadFromFile(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) 
	{
		throw std::runtime_error("Cannot open file for reading: " + filename);
	}

	
	int new_rows, new_cols;
	if (!(file >> new_rows >> new_cols)) 
	{
		throw std::runtime_error("Invalid file format: cannot read matrix dimensions");
	}

	
	if (new_rows <= 0 || new_cols <= 0) 
	{
		throw std::runtime_error("Invalid matrix dimensions in file");
	}

	
	delete[] m;
	rows = new_rows;
	cols = new_cols;
	m = new T[rows * cols];

	
	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < cols; j++) 
		{
			T value;
			if (!(file >> value)) 
			{
				throw std::runtime_error("Invalid file format: not enough matrix elements");
			}
			Set(value, i, j);
		}
	}

	file.close();
}

template<typename T>
inline int TMatrix<T>::CountOccurrenses(const T& value)
{
	int count = 0;
	int size = cols * rows;
	for (int i = 0; i < size; ++i)
	{
		if (m[i] == value)
		{
			++count;
		}
	}
	return count;
}

template<typename T>
inline std::vector<typename TMatrix<T>::Coordinates> TMatrix<T>::FindAll(const T& value)
{
	std::vector<Coordinates> positions;
	int index = 0;
	for (Iterator k = begin(); k != end(); ++k, ++index)
	{
		if (*k == value)
		{
			int i = index / cols;
			int j = index % cols;
			Coordinates kor{ i, j };
			positions.push_back(kor);
		}
	}
	return positions;
}

template<typename T>
inline TMatrix<T> TMatrix<T>::Power(int exponent)
{
	if (rows != cols)
	{
		throw std::invalid_argument("Matrix must be square for exponentiation");
	}

	if (exponent < 0)
	{
		throw std::invalid_argument("Exponent can't be negative");
	}

	if (exponent == 0)
	{
		TMatrix<T> result(rows, cols);
		for (int i = 0; i < rows; i++)
		{
			result.Set(T(1), i, i);
		}
		return result;
	}

	if (exponent == 1)
	{
		return TMatrix<T>(*this);
	}

	TMatrix<T> result = *this;
	TMatrix<T> buf = *this;
	int n = exponent - 1;

	while (n > 0)
	{
		if (n % 2 == 1)
		{
			result = result * buf;
			n = n - 1;
		}
		else
		{
			buf = buf * buf;
			n = n / 2;
		}
	}

	return result;
}

template<typename T>
inline T TMatrix<T>::FirstNorm()
{
	if (rows == 0 || cols == 0)
	{
		return T(0);
	}
	T MaxSum(0);
	for (int j = 0; j < cols; j++)
	{
		T Sum(0);
		for (int i = 0; i < rows; i++)
		{
			T p = Get(i, j);
			Sum += std::abs(p);
		}
		if (Sum > MaxSum)
		{
			MaxSum = Sum;
		}
	}
	return MaxSum;
}

template<typename T>
inline double TMatrix<T>::SecondNorm()
{
	double sum = 0.0;
	for (auto p = begin(); p != end(); ++p)
	{
		double value = *p;
		sum += value * value;
	}
	return std::sqrt(sum);
}

template<typename T>
inline double TMatrix<T>::HelderNorm(double p)
{
	if (p < 1.0) 
	{
		throw std::invalid_argument("p must be >= 1.0");
	}

	double sum = 0.0;
	for (auto it = begin(); it != end(); ++it) 
	{
		sum += std::pow(std::abs((*it)), p);
	}
	return std::pow(sum, 1.0 / p);
}

template<typename T>
inline T TMatrix<T>::InfinityNorm()
{
	if (rows == 0 || cols == 0) 
	{
		return T(0);
	}

	T maxSum = T(0);
	for (int i = 0; i < rows; ++i) 
	{
		T rowSum = T(0);
		for (int j = 0; j < cols; ++j) 
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

template<typename T>
std::ostream& operator<<(std::ostream& os, const TMatrix<T>& mat)
{
	os << "Matrix " << mat.rows << "x" << mat.cols << ":\n";
	for (int i = 0; i < mat.rows; i++) 
	{
		for (int j = 0; j < mat.cols; j++) 
		{
			os << mat.Get(i, j);
			if (j < mat.cols - 1) 
			{
				os << " ";
			}
		}
		os << "\n";
	}
	return os;
}

template<typename T>
std::istream& operator>>(std::istream& is, TMatrix<T>& mat)
{
	int rows, cols;

	
	std::cout << "Enter matrix dimensions (rows cols): ";
	if (!(is >> rows >> cols)) 
	{
		throw std::runtime_error("Failed to read matrix dimensions");
	}

	if (rows <= 0 || cols <= 0) 
	{
		throw std::invalid_argument("Matrix dimensions must be positive");
	}

	
	TMatrix<T> temp(rows, cols);

	
	std::cout << "Enter " << rows << "x" << cols << " matrix elements:\n";
	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < cols; j++) 
		{
			T value;
			if (!(is >> value)) 
			{
				throw std::runtime_error("Failed to read matrix element");
			}
			temp.Set(value, i, j);
		}
	}

	
	mat = std::move(temp);
	return is;
}
