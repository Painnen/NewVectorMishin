#pragma once

#pragma once
#include <vector>
#include <functional>
#include <string>
#include<stdexcept>

template <class T>
class SparceMatrix
{
private:
    size_t dimension;
    size_t capacity;
    size_t size;
    T* values;
    size_t* rows;
    size_t* columns;

public:
   
    SparceMatrix();
    explicit SparceMatrix(size_t dim);
    SparceMatrix(size_t dim, size_t cap);
    SparceMatrix(const SparceMatrix& copy);
    SparceMatrix(SparceMatrix&& copy);
    ~SparceMatrix();

    
    T Get(size_t i, size_t j) const;
    void Set(const T& value, size_t i, size_t j);

    
    SparceMatrix operator+(const SparceMatrix& mx) const;
    SparceMatrix operator-(const SparceMatrix& mx) const;
    SparceMatrix operator*(const SparceMatrix& mx) const;
    SparceMatrix operator*(const T& scalar) const;
    SparceMatrix operator/(const T& scalar) const;
    SparceMatrix& operator=(const SparceMatrix& mx);
    bool operator==(const SparceMatrix& mx) const;
    bool operator!=(const SparceMatrix& mx) const;

   
  
    void SaveToFile(const std::string& filename) const;
    void LoadFromFile(const std::string& filename);

  
    friend std::ostream& operator<<(std::ostream& os, const SparceMatrix& mx);
    friend std::istream& operator>>(std::istream& is, SparceMatrix& mx);
 

    
    class Iterator
    {
    private:
        T* ptr;
        size_t current_index;
        const SparceMatrix* matrix;
    public:
        Iterator(T* p = nullptr, size_t idx = 0, const SparceMatrix* m = nullptr);
        Iterator& operator++();
        T& operator*() const;
        T* operator->() const;
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
    };

    Iterator begin();
    Iterator end();

   
    void ApplyLambda(std::function<void(T&)> lambda);

  
    int CountOccurrences(const T& value) const;
    std::vector<std::pair<size_t, size_t>> FindAll(const T& value) const;

    SparceMatrix Power(int exponent) const;

  
    T FirstNorm() const;
    double SecondNorm() const;
    double HelderNorm(double p) const;
    T InfinityNorm() const;

  
    size_t GetDimension() const;
    size_t GetSize() const;
    size_t GetCapacity() const;
    


    void RemoveElement(size_t index);
};

template<class T>
inline SparceMatrix<T>::SparceMatrix()
    : dimension(0), capacity(0), size(0), values(nullptr), rows(nullptr), columns(nullptr)
{}

template<class T>
inline SparceMatrix<T>::SparceMatrix(size_t dim)
    : dimension(dim), capacity(10), size(0)  
{
    values = new T[capacity];
    rows = new size_t[capacity];
    columns = new size_t[capacity];
}

template<class T>
inline SparceMatrix<T>::SparceMatrix(size_t dim, size_t cap)
    : dimension(dim), capacity(cap), size(0)
{
    if (capacity > 0) 
    {
        values = new T[capacity];
        rows = new size_t[capacity];
        columns = new size_t[capacity];
    }
    else 
    {
        values = nullptr;
        rows = nullptr;
        columns = nullptr;
    }
}

template<class T>
inline size_t SparceMatrix<T>::GetDimension() const
{
    return dimension;
}

template<class T>
inline size_t SparceMatrix<T>::GetSize() const
{
    return size;
}

template<class T>
inline size_t SparceMatrix<T>::GetCapacity() const
{
    return capacity;
}

template<class T>
inline SparceMatrix<T>::SparceMatrix(const SparceMatrix& copy)
    : dimension(copy.dimension), capacity(copy.capacity), size(copy.size)
{
    if (capacity > 0) 
    {
        values = new T[capacity];
        rows = new size_t[capacity];
        columns = new size_t[capacity];

        for (size_t i = 0; i < size; ++i) 
        {
            values[i] = copy.values[i];
            rows[i] = copy.rows[i];
            columns[i] = copy.columns[i];
        }
    }
    else 
    {
        values = nullptr;
        rows = nullptr;
        columns = nullptr;
    }
}

template<class T>
inline SparceMatrix<T>::SparceMatrix(SparceMatrix&& copy)
    : dimension(copy.dimension), capacity(copy.capacity), size(copy.size),
    values(copy.values), rows(copy.rows), columns(copy.columns)
{
    copy.dimension = 0;
    copy.capacity = 0;
    copy.size = 0;
    copy.values = nullptr;
    copy.rows = nullptr;
    copy.columns = nullptr;
}

template<class T>
inline SparceMatrix<T>::~SparceMatrix()
{
    delete[] values;
    delete[] rows;
    delete[] columns;
}

template<class T>
inline void SparceMatrix<T>::RemoveElement(size_t index)
{
    if (index >= size) return;

    
    for (size_t i = index; i < size - 1; i++) 
    {
        values[i] = values[i + 1];
        rows[i] = rows[i + 1];
        columns[i] = columns[i + 1];
    }
    --size;
}


template<class T>
inline T SparceMatrix<T>::Get(size_t i, size_t j) const
{
    for (int k = 0; k < size; k++)
    {
        if (rows[k] == i && columns[k] == j)
        {
                return values[k];
        }
    }
    return T(0);
}

template<class T>
inline void SparceMatrix<T>::Set(const T& value, size_t i, size_t j)
{
    
    if (i >= dimension || j >= dimension) 
    {
        throw std::out_of_range("Matrix index out of range");
    }

    
    for (size_t k = 0; k < size; k++)
    {
        if (rows[k] == i && columns[k] == j)
        {
            if (value == T(0)) 
            {
                RemoveElement(k);
            }
            else 
            {
                values[k] = value;
            }
            return;
        }
    }

    
    if (value != T(0)) 
    {
        if (size >= capacity) 
        {
            size_t new_capacity = size + static_cast<size_t>(log2(size)) + 1;

            
            T* new_values = new T[new_capacity];
            size_t* new_rows = new size_t[new_capacity];
            size_t* new_columns = new size_t[new_capacity];

            
            for (size_t a = 0; a < size; a++)
            {
                new_values[a] = values[a];
                new_rows[a] = rows[a];
                new_columns[a] = columns[a];
            }

            
            delete[] values;
            delete[] rows;
            delete[] columns;

            
            values = new_values;
            rows = new_rows;
            columns = new_columns;
            capacity = new_capacity;
        }

        
        values[size] = value;
        rows[size] = i;
        columns[size] = j;
        ++size;
    }
}


template<class T>
inline SparceMatrix<T> SparceMatrix<T>::operator+(const SparceMatrix& mx) const
{
    if (dimension != mx.dimension)
    {
        throw std::invalid_argument("different dimensions");
    }

    SparceMatrix result(dimension, capacity + mx.capacity);

    
    for (size_t i = 0; i < size; i++)
    {
        result.Set(values[i], rows[i], columns[i]);
    }

    
    for (size_t i = 0; i < mx.size; i++)
    {
        size_t row = mx.rows[i];
        size_t col = mx.columns[i];
        T current_val = result.Get(row, col); 
        result.Set(current_val + mx.values[i], row, col);
    }

    return result;
}

template<class T>
inline SparceMatrix<T> SparceMatrix<T>::operator-(const SparceMatrix& mx) const
{
    if (dimension != mx.dimension)
    {
        throw std::invalid_argument("different dimensions");
    }
    SparceMatrix result(dimension, capacity);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (columns[i] == mx.columns[j] && rows[i] == mx.rows[j])
            {
                result.Set(values[i] - mx.values[j], rows[i], columns[i]);
            }
            else
            {
                result.Set(values[i], rows[i], columns[i]);
            }
        }
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (mx.columns[i] != columns[j] || mx.rows[i] != rows[j])
            {
                result.Set(-mx.values[j], mx.rows[i], mx.columns[i]);
            }
        }
    }
    return result;
}

template<class T>
inline SparceMatrix<T> SparceMatrix<T>::operator*(const SparceMatrix& mx) const
{
    if (dimension != mx.dimension)
    {
        throw std::invalid_argument("Matrix dimensions must match for multiplication");
    }

    SparceMatrix result(dimension, size * mx.size); 

    // Умножение матриц: C[i][j] = sum_k (A[i][k] * B[k][j])
    for (size_t i = 0; i < dimension; i++)
    {
        for (size_t j = 0; j < dimension; j++)
        {
            T sum = T(0);

            for (size_t k_idx = 0; k_idx < size; k_idx++)
            {
                
                if (rows[k_idx] == i)
                {
                    size_t k = columns[k_idx]; 
                    T a_val = values[k_idx];

                    
                    T b_val = mx.Get(k, j);
                    if (b_val != T(0))
                    {
                        sum += a_val * b_val;
                    }
                }
            }

            
            if (sum != T(0))
            {
                result.Set(sum, i, j);
            }
        }
    }

    return result;
}

template<class T>
inline SparceMatrix<T> SparceMatrix<T>::operator*(const T& scalar) const
{
    SparceMatrix result(*this);
    for (int i = 0; i < size; i++)
    {
        result.values[i] = values[i] * scalar;
    }
    return result;
}

template<class T>
inline SparceMatrix<T> SparceMatrix<T>::operator/(const T& scalar) const
{
    if (scalar == T())
    {
        throw std::invalid_argument("division by 0");
    }
    SparceMatrix result(*this);
    for (int i = 0; i < size; i++)
    {
        result.values[i] = values[i] / scalar;
    }
    return result;
}

template<class T>
inline SparceMatrix<T>& SparceMatrix<T>::operator=(const SparceMatrix& mx)
{
    if (this != &mx) 
    {
        
        delete[] values;
        delete[] rows;
        delete[] columns;

        
        dimension = mx.dimension;
        capacity = mx.capacity;
        size = mx.size;

        
        if (capacity > 0) 
        {
            values = new T[capacity];
            rows = new size_t[capacity];
            columns = new size_t[capacity];

            
            for (size_t i = 0; i < size; ++i) 
            {
                values[i] = mx.values[i];
                rows[i] = mx.rows[i];
                columns[i] = mx.columns[i];
            }
        }
        else 
        {
            values = nullptr;
            rows = nullptr;
            columns = nullptr;
        }
    }
    return *this;
}



template<class T>
inline bool SparceMatrix<T>::operator==(const SparceMatrix& mx) const
{
    if (dimension != mx.dimension)
    {
        return false;
    }
    if (size != mx.size)
    {
        return false;
    }
    
    for (size_t idx = 0; idx < size; ++idx) 
    {
        size_t i = rows[idx];
        size_t j = columns[idx];
        if (values[idx] != mx.Get(i, j)) 
        {
            return false;
        }
    }
    return true;
}

template<class T>
inline bool SparceMatrix<T>::operator!=(const SparceMatrix& mx) const
{
    return !(*this == mx);
}

template<class T>
inline void SparceMatrix<T>::SaveToFile(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) 
    {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    
    file << dimension << "\n";

    
    file << size << "\n";

    
    for (size_t i = 0; i < size; i++) 
    {
        file << rows[i] << " " << columns[i] << " " << values[i] << "\n";
    }

    file.close();
}

template<class T>
inline void SparceMatrix<T>::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) 
    {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    
    size_t new_dimension;
    file >> new_dimension;

    
    size_t new_size;
    file >> new_size;

    
    SparceMatrix temp(new_dimension, new_size);

    
    for (size_t i = 0; i < new_size; i++) 
    {
        size_t row, col;
        T value;
        file >> row >> col >> value;

        if (row >= new_dimension || col >= new_dimension) 
        {
            throw std::runtime_error("Invalid matrix indices in file");
        }

        temp.Set(value, row, col);
    }

    
    *this = std::move(temp);
    file.close();
}

template<class T>
std::ostream& operator<<(std::ostream& os, const SparceMatrix<T>& mx)
{
    
    os << "Sparse Matrix " << mx.dimension << "x" << mx.dimension;
    os << " (" << mx.size << " non-zero elements)\n";

    
    for (size_t i = 0; i < mx.size; i++) 
    {
        os << "[" << mx.rows[i] << "," << mx.columns[i] << "] = " << mx.values[i] << "\n";
    }

    
    if (mx.dimension <= 10) 
    {
        os << "\nMatrix view:\n";
        for (size_t i = 0; i < mx.dimension; i++) 
        {
            for (size_t j = 0; j < mx.dimension; j++) 
            {
                os << mx.Get(i, j) << " ";
            }
            os << "\n";
        }
    }

    return os;
}

template<class T>
std::istream& operator>>(std::istream& is, SparceMatrix<T>& mx)
{
    
    size_t dim;
    std::cout << "Enter matrix dimension: ";
    is >> dim;

    if (dim == 0) 
    {
        throw std::invalid_argument("Matrix dimension must be positive");
    }

    
    SparceMatrix<T> temp(dim);

    
    std::cout << "Enter non-zero elements (format: row column value). Enter -1 -1 0 to finish:\n";

    while (true) 
    {
        size_t row, col;
        T value;

        std::cout << "> ";
        is >> row >> col >> value;

        
        if (row == static_cast<size_t>(-1) && col == static_cast<size_t>(-1)) {
            break;
        }

        if (row >= dim || col >= dim) 
        {
            std::cout << "Error: indices must be in range [0, " << dim - 1 << "]. Try again.\n";
            continue;
        }

        temp.Set(value, row, col);
    }

    mx = std::move(temp);
    return is;
}

template<class T>
inline SparceMatrix<T>::Iterator::Iterator(T* p, size_t idx, const SparceMatrix* m)
    : ptr(p), current_index(idx), matrix(m)
{
}

template<class T>
inline typename SparceMatrix<T>::Iterator& SparceMatrix<T>::Iterator::operator++()
{
    if (ptr && current_index < matrix->size) 
    {
        current_index++;
        if (current_index < matrix->size) 
        {
            ptr = &matrix->values[current_index];
        }
        else 
        {
            ptr = nullptr;
        }
    }
    return *this;
}

template<class T>
inline T& SparceMatrix<T>::Iterator::operator*() const
{
    if (!ptr || current_index >= matrix->size) 
    {
        throw std::runtime_error("Dereferencing invalid iterator");
    }
    return *ptr;
}

template<class T>
inline T* SparceMatrix<T>::Iterator::operator->() const
{
    if (!ptr || current_index >= matrix->size) 
    {
        throw std::runtime_error("Accessing through invalid iterator");
    }
    return ptr;
}

template<class T>
inline bool SparceMatrix<T>::Iterator::operator==(const Iterator& other) const
{
    return current_index == other.current_index && matrix == other.matrix;
}

template<class T>
inline bool SparceMatrix<T>::Iterator::operator!=(const Iterator& other) const
{
    return !(*this == other);
}

template<class T>
typename SparceMatrix<T>::Iterator SparceMatrix<T>::begin()
{
    if (size == 0) {
        return Iterator(nullptr, 0, this);
    }
    return Iterator(&values[0], 0, this);
}

template<class T>
typename SparceMatrix<T>::Iterator SparceMatrix<T>::end()
{
    return Iterator(nullptr, size, this);
}

// ApplyLambda implementation
template<class T>
inline void SparceMatrix<T>::ApplyLambda(std::function<void(T&)> lambda)
{
    for (size_t i = 0; i < size; i++) 
    {
        lambda(values[i]);
    }
}

// CountOccurrences implementation
template<class T>
inline int SparceMatrix<T>::CountOccurrences(const T& value) const
{
    int count = 0;
    for (size_t i = 0; i < size; i++) 
    {
        if (values[i] == value) 
        {
            count++;
        }
    }
    return count;
}

// FindAll implementation
template<class T>
inline std::vector<std::pair<size_t, size_t>> SparceMatrix<T>::FindAll(const T& value) const
{
    std::vector<std::pair<size_t, size_t>> positions;
    for (size_t i = 0; i < size; i++) 
    {
        if (values[i] == value) 
        {
            positions.push_back({ rows[i], columns[i] });
        }
    }
    return positions;
}

// Power implementation
template<class T>
inline SparceMatrix<T> SparceMatrix<T>::Power(int exponent) const
{
    if (dimension == 0) 
    {
        return SparceMatrix<T>();
    }

    if (exponent < 0) {
        throw std::invalid_argument("Exponent cannot be negative for sparse matrices");
    }

    if (exponent == 0) {
        // Возвращаем единичную матрицу
        SparceMatrix<T> result(dimension);
        for (size_t i = 0; i < dimension; i++) {
            result.Set(T(1), i, i);
        }
        return result;
    }

    if (exponent == 1) 
    {
        return *this;
    }

    // Быстрое возведение в степень
    SparceMatrix<T> result = *this;
    SparceMatrix<T> base = *this;
    int n = exponent - 1;

    while (n > 0) {
        if (n % 2 == 1) 
        {
            result = result * base;
            n--;
        }
        else {
            base = base * base;
            n /= 2;
        }
    }

    return result;
}

// Norms implementation
template<class T>
inline T SparceMatrix<T>::FirstNorm() const
{
    if (dimension == 0) 
    {
        return T(0);
    }

    T maxSum = T(0);
    for (size_t j = 0; j < dimension; j++) 
    {
        T colSum = T(0);
        for (size_t i = 0; i < dimension; i++) 
        {
            colSum += std::abs(Get(i, j));
        }
        if (colSum > maxSum) 
        {
            maxSum = colSum;
        }
    }
    return maxSum;
}

template<class T>
inline double SparceMatrix<T>::SecondNorm() const
{
    double sum = 0.0;
    for (size_t i = 0; i < size; i++) 
    {
        double val = static_cast<double>(values[i]);
        sum += val * val;
    }
    return std::sqrt(sum);
}

template<class T>
inline double SparceMatrix<T>::HelderNorm(double p) const
{
    if (p < 1.0) 
    {
        throw std::invalid_argument("p must be >= 1.0");
    }

    double sum = 0.0;
    for (size_t i = 0; i < size; i++) 
    {
        sum += std::pow(std::abs(static_cast<double>(values[i])), p);
    }
    return std::pow(sum, 1.0 / p);
}

template<class T>
inline T SparceMatrix<T>::InfinityNorm() const
{
    if (dimension == 0) 
    {
        return T(0);
    }

    T maxSum = T(0);
    for (size_t i = 0; i < dimension; i++) 
    {
        T rowSum = T(0);
        for (size_t j = 0; j < dimension; j++) 
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