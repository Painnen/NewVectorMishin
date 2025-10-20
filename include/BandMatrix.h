#pragma once
#include <functional>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cmath>
#include"TMatrix.h"

template <class T>
class BandMatrix
{
	size_t dimension;
	TMatrix<T> data;
	size_t diags;

public:
	BandMatrix();
	BandMatrix(size_t dim, size_t d);
	BandMatrix(BandMatrix& copy);
	BandMatrix(BandMatrix&& copy);
	~BandMatrix();

	size_t GetDimension() const { return dimension; }
	size_t GetDiags() const { return diags; }

	BandMatrix operator+(BandMatrix& other);
	BandMatrix operator-(BandMatrix& other);
	BandMatrix operator*(T& scalar);
	BandMatrix operator/(T& scalar);
	BandMatrix& operator=(BandMatrix& other);
	bool operator==(BandMatrix& other);
	bool operator!=(BandMatrix& other);

	void Set(const T& value, size_t i, size_t j) 
	{
		if (i < dimension && j < diags) 
		{
			data.Set(value, i, j);
		}
	}

	T Get(size_t i, size_t j) const 
	{
		if (i < dimension && j < diags) 
		{
			return data.Get(i, j);
		}
		return T(0);
	}

	struct Coordinates
	{
		int row_;
		int col_;
	};


	void ApplyLambda(std::function<void(T&)> lambda);


	void SaveToFile(const std::string& filename) const;
	void LoadFromFile(const std::string& filename);

	template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const BandMatrix<U>& mat);

    template<typename U>
    friend std::istream& operator>>(std::istream& is, BandMatrix<U>& mat);

	int CountOccurrenses(const T& value);
	std::vector<Coordinates> FindAll(const T& value);
	T FirstNorm();
	double SecondNorm();
	double HelderNorm(double p);
	T InfinityNorm();

};

template<class T>
inline BandMatrix<T>::BandMatrix() : dimension(0), diags(0), data()
{}

template<class T>
inline BandMatrix<T>::BandMatrix(size_t dim, size_t d) : dimension(dim), diags(d), data(dim, d)
{}

template<class T>
inline BandMatrix<T>::BandMatrix(BandMatrix& copy) : dimension(copy.dimension), diags(copy.diags), data(copy.data)
{}

template<class T>
inline BandMatrix<T>::BandMatrix(BandMatrix&& copy) : data(copy.data), diags(copy.diags), dimension(copy.dimension)
{
	copy.diags = 0;
	copy.dimension = 0;
	copy.data.SetMatrix(nullptr);
}

template<class T>
inline BandMatrix<T>::~BandMatrix()
{}

template<class T>
inline BandMatrix<T> BandMatrix<T>::operator+(BandMatrix& other)
{
	if (dimension != other.dimension)
	{
		throw std::invalid_argument("different dimensions");
	}
	if (diags == other.diags)
	{
		BandMatrix result(dimension, diags);
		result.data = data + other.data;
		return result;
	}
	BandMatrix result(dimension, std::max(diags, other.diags));
	result.data.Fill(0);
	{
		if (diags > other.diags)
		{
			for (int i = 0; i < dimension; i++)
			{
				for (int j = 0; j < other.diags; j++)
				{
					result.data.Set(other.data.Get(i, j), i, j);
				}
			}
			result.data = result.data + data;
		}
		else
		{
			for (int i = 0; i < dimension; i++)
			{
				for (int j = 0; j < diags; j++)
				{
					result.data.Set(data.Get(i, j), i, j);
				}
			}
			result.data = result.data + other.data;
		}
	}
	return result;
}

template<class T>
inline BandMatrix<T> BandMatrix<T>::operator-(BandMatrix& other)
{
	if (dimension != other.dimension)
	{
		throw std::invalid_argument("different dimensions");
	}
	if (diags == other.diags)
	{
		BandMatrix result(dimension, diags);
		result.data = data + other.data;
		return result;
	}
	BandMatrix result(dimension, std::max(diags, other.diags));
	result.data.Fill(0);
	{
		if (diags > other.diags)
		{
			for (int i = 0; i < dimension; i++)
			{
				for (int j = 0; j < other.diags; j++)
				{
					result.data.Set(-(other.data.Get(i, j)), i, j);
				}
			}
			result.data = result.data + data;
		}
		else
		{
			for (int i = 0; i < dimension; i++)
			{
				for (int j = 0; j < diags; j++)
				{
					result.data.Set(data.Get(i, j), i, j);
				}
			}
			result.data = result.data - other.data;
		}
	}
	return result;
}

template<class T>
inline BandMatrix<T> BandMatrix<T>::operator*(T& scalar)
{
	BandMatrix result(*this);
	result.data = data * scalar;
	return result;
}

template<class T>
inline BandMatrix<T> BandMatrix<T>::operator/(T& scalar)
{
	if (scalar == 0)
	{
		throw std::invalid_argument("division by 0");
	}
	BandMatrix result(*this);
	result.data = data * scalar;
	return result;
}

template<class T>
inline BandMatrix<T>& BandMatrix<T>::operator=(BandMatrix& other)
{
	data = other.data;
	dimension = other.dimension;
	diags = other.diags;
	return (*this);
}

template<class T>
inline bool BandMatrix<T>::operator==(BandMatrix& other)
{
	if (dimension != other.dimension || diags != other.diags)
	{
		return false;
	}
	if (data != other.data)
	{
		return false;
	}
	return true;
}

template<class T>
inline bool BandMatrix<T>::operator!=(BandMatrix& other)
{
	return !(*this == other);
}

template<class T>
inline void BandMatrix<T>::SaveToFile(const std::string& filename) const
{
	std::ofstream file(filename);
	if (!file.is_open()) 
	{
		throw std::runtime_error("Cannot open file: " + filename);
	}

	
	file << dimension << " " << diags << "\n";

	
	for (size_t i = 0; i < dimension; i++) 
	{
		for (size_t j = 0; j < diags; j++) 
		{
			file << data.Get(i, j) << " ";
		}
		file << "\n";
	}

	file.close();
}

template<class T>
inline void BandMatrix<T>::LoadFromFile(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) 
	{
		throw std::runtime_error("Cannot open file: " + filename);
	}

	size_t new_dimension, new_diags;
	file >> new_dimension >> new_diags;

	if (new_dimension == 0) 
	{
		throw std::runtime_error("Invalid matrix dimension in file");
	}

	
	BandMatrix temp(new_dimension, new_diags);

	
	for (size_t i = 0; i < new_dimension; i++) 
	{
		for (size_t j = 0; j < new_diags; j++) 
		{
			T value;
			file >> value;
			temp.data.Set(value, i, j);
		}
	}

	
	*this = std::move(temp);
	file.close();
}


template<class T>
std::ostream& operator<<(std::ostream& os, const BandMatrix<T>& mx)
{
	
	os << "BandMatrix " << mx.dimension << "x" << mx.dimension << " (band width: " << mx.diags << ")\n";
	os << "Stored data:\n";

	for (size_t i = 0; i < mx.dimension; i++) 
	{
		for (size_t j = 0; j < mx.diags; j++) 
		{
			os << mx.data.Get(i, j) << " ";
		}
		os << "\n";
	}
	return os;
}

template<class T>
std::istream& operator>>(std::istream& is, BandMatrix<T>& mx)
{
	
	size_t dim, band_width;
	is >> dim >> band_width;

	BandMatrix<T> temp(dim, band_width);

	for (size_t i = 0; i < dim; i++) 
	{
		for (size_t j = 0; j < band_width; j++) 
		{
			T value;
			is >> value;
			temp.data.Set(value, i, j);
		}
	}

	mx = std::move(temp);
	return is;
}

template<class T>
inline void BandMatrix<T>::ApplyLambda(std::function<void(T&)> lambda)
{
	for (size_t i = 0; i < dimension; i++) 
	{
		for (size_t j = 0; j < diags; j++) 
		{
			T value = data.Get(i, j);
			lambda(value);
			data.Set(value, i, j);
		}
	}
}


template<class T>
inline T BandMatrix<T>::FirstNorm()
{
	if (dimension == 0) {
		return T(0);
	}

	T maxSum = T(0);
	for (size_t j = 0; j < dimension; j++) 
	{
		T colSum = T(0);
		for (size_t i = 0; i < dimension; i++) 
		{
			
			if (std::abs(static_cast<int>(i) - static_cast<int>(j)) <= static_cast<int>(diags - 1)) 
			{
				size_t band_index = (j >= i) ? (j - i) : (diags - 1 + i - j);
				colSum += std::abs(data.Get(i, band_index));
			}
		}
		if (colSum > maxSum) 
		{
			maxSum = colSum;
		}
	}
	return maxSum;
}

template<class T>
inline double BandMatrix<T>::SecondNorm()
{
	double sum = 0.0;
	for (size_t i = 0; i < dimension; i++) 
	{
		for (size_t j = 0; j < diags; j++) 
		{
			double val = static_cast<double>(data.Get(i, j));
			sum += val * val;
		}
	}
	return std::sqrt(sum);
}

template<class T>
inline double BandMatrix<T>::HelderNorm(double p)
{
	if (p < 1.0) {
		throw std::invalid_argument("p must be >= 1.0");
	}

	double sum = 0.0;
	for (size_t i = 0; i < dimension; i++) 
	{
		for (size_t j = 0; j < diags; j++) 
		{
			sum += std::pow(std::abs(static_cast<double>(data.Get(i, j))), p);
		}
	}
	return std::pow(sum, 1.0 / p);
}

template<class T>
inline T BandMatrix<T>::InfinityNorm()
{
	if (dimension == 0) 
	{
		return T(0);
	}

	T maxSum = T(0);
	for (size_t i = 0; i < dimension; i++) 
	{
		T rowSum = T(0);
		for (size_t j = 0; j < diags; j++) 
		{
			rowSum += std::abs(data.Get(i, j));
		}
		if (rowSum > maxSum) 
		{
			maxSum = rowSum;
		}
	}
	return maxSum;
}
