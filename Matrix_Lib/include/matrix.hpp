#ifndef MATRIX_LIB
#define MATRIX_LIB

#include <iostream>
#include <iterator>
#include <iomanip>
#include <type_traits>
#include <functional>

#include "comparison.hpp"

namespace Matrix_Algebra
{

template<typename T = double> class Matrix
{

    size_t n_ = 0, m_ = 0;
    T** rows_ = nullptr;

    struct ProxyRow
    {
        T *row = nullptr;
        T& operator[](int n) { return row[n]; }
        const T &operator[](int n) const { return row[n]; }
    };

public:

    Matrix(size_t n, size_t m, T value = T{}) : n_{n}, m_{m}, rows_{new T* [m]}
    {
        for (auto i = 0; i < m_; ++i)
        {
            rows_[i] = new T [n_];
            std::fill (rows_[i], rows_[i] + n_, value);
        }
    }

    //  Constructor of matrix (n x m) from sequence
    //  explanation: end has same sence as end of std::vector
    template<std::input_iterator It> 
    Matrix (size_t n, size_t m, const It start, const It end) : n_{n}, m_{m}, rows_{new T* [m]}
    {
        for (auto i = 0; i < m_; ++i)
        {
            rows_[i] = new T[n_];
            std::copy (start + i * n_, start + (i + 1) * n_, rows_[i]);
        }
    }

    //  Constructor of vector (1 x n) from sequence
    //  explanation: end has same sence as end of std::vector
    template<std::input_iterator It>
    Matrix (size_t n, const It start, const It end) : n_{n}, m_{1}, rows_{new T* [m_]}
    {
        *rows_ = new T[n_];
        std::copy (start, end, *rows_);
    }

    //  Constructor from nested initializer list
    Matrix (std::initializer_list<std::initializer_list<T>> input_list) : m_{input_list.size()}, 
                                                                          rows_{new T* [input_list.size()]}
    {
        //  finding out max length of string
        auto input_list_ptr = input_list.begin();
        n_ = input_list_ptr[0].size();
        for (int i = 0; i < m_; ++i)
        {
            if (n_ < input_list_ptr[i].size())
                n_ = input_list_ptr[i].size();
        }

        for (int i = 0; i < m_; ++i)
        {
            rows_[i] = new T[n_];
            
            auto input_list_i_ptr = input_list_ptr[i].begin();
            auto input_list_i_len = input_list_ptr[i].size();
            std::copy (input_list_i_ptr, input_list_i_ptr + input_list_i_len, rows_[i]);

            if (input_list_i_len < n_)
                std::fill(rows_[i] + input_list_i_len, rows_[i] + n_, T(0));
        }
    }

    //  Constructor from single initializer list
    Matrix (std::initializer_list<T> input_list) : m_{1}, n_{input_list.size()}, rows_{new T* [1]}
    {
        *rows_ = new T[n_];
        std::copy (input_list.begin(), input_list.end(), *rows_);
    }

    Matrix(const Matrix &rhs) : n_{rhs.n_}, m_{rhs.m_}, rows_{new T *[rhs.m_]}
    {
        for (auto i = 0; i < m_; ++i)
        {
            rows_[i] = new T [n_];
            std::copy (rhs.rows_[i], rhs.rows_[i] + n_, rows_[i]);
        }
    }

    Matrix(Matrix &&rhs) noexcept : n_{std::exchange(rhs.n_, 0)},
                                    m_{std::exchange(rhs.m_, 0)},
                                    rows_{std::exchange(rhs.rows_, nullptr)} {}

    Matrix& operator= (const Matrix& rhs)
    {
        auto matrix{rhs};
        std::swap(*this, matrix);

        return *this;
    }

    Matrix& operator=(Matrix &&rhs) noexcept
    {
        std::swap(n_, rhs.n_);
        std::swap(m_, rhs.m_);
        std::swap(rows_, rhs.rows_);
        
        return *this;
    }

    ~Matrix() 
    {
        for (auto i = 0; i < m_; ++i)
            delete [] rows_[i];
        delete [] rows_;
    }

//  Access methods and selectors

    ProxyRow operator[](int m) { return ProxyRow {rows_[m]}; }
    const ProxyRow operator[](int m) const { return ProxyRow {rows_[m]}; }

    size_t get_rows() const { return m_; }
    size_t get_cols() const { return n_; }

    T trace() const 
    {
        T trace_ = 0;
        for (auto i = 0; i < n_; ++i)
            trace_ += rows_[i][i];
        return trace_;
    }

    void dump(std::ostream& os = std::cout) const
    {
        std::cout << '\n';
        for (auto i = 0; i < m_; ++i)
        {
            os << '\t';
            for (auto j = 0; j < n_; ++j)
                os << "|" << std::setw(5) << std::left << rows_[i][j];
            os << "|\n";
        }
    }

    bool operator== (const Matrix& rhs) const
    {
        if (this == &rhs)
            return true;

        if (m_ != rhs.m_ || n_ != rhs.n_)
            return false;

        if (std::is_integral<T>::value == true)
        {
            for (auto i = 0; i < m_; ++i)
            {
                if (!std::equal(rows_[i], rows_[i] + n_, rhs.rows_[i]))
                    return false;
            }
        }
        else
        {
            for (auto i = 0; i < m_; ++i)
            {
                if (!std::equal(rows_[i], rows_[i] + n_, rhs.rows_[i],
                                [](T first, T second)
                                { return cmp::are_equal(first, second, cmp::epsilon); }))
                    return false;
            }
        }

        return true;
    }
    bool operator!=(const Matrix &rhs) const { return !(*this == rhs); }

    Matrix operator- ()
    {
        Matrix tmp = *this;
        for (auto i = 0; i < m_; ++i)
            std::transform (rows_[i], rows_[i] + n_, tmp.rows_[i], std::negate());

        return tmp;
    }
    Matrix& inverse_sign ()
    {
        for (auto i = 0; i < m_; ++i)
            std::transform (rows_[i], rows_[i] + n_, rows_[i], std::negate());

        return *this;
    }

    Matrix& operator+= (const Matrix& rhs)
    {
        if (m_ != rhs.m_ || n_ != rhs.n_)
            throw std::logic_error("Inappropriate size for matrix addition");

        for (auto i = 0; i < m_; ++i)
            std::transform (rows_[i], rows_[i] + n_, rhs.rows_[i], rows_[i], std::plus());

        return *this;
    }

    Matrix& operator-= (const Matrix& rhs)
    {
        if (m_ != rhs.m_ || n_ != rhs.n_)
            throw std::logic_error("Inappropriate size for matrix subtraction");

        for (auto i = 0; i < m_; ++i)
            std::transform (rows_[i], rows_[i] + n_, rhs.rows_[i], rows_[i], std::minus());

        return *this;
    }

    Matrix & operator*=(T value)
    {
        for (auto i = 0; i < m_; ++i)
            std::transform(
                rows_[i], rows_[i] + n_, rows_[i], [value](T arg) { return arg *= value; });

        return *this;
    }

    Matrix& operator/= (T value)
    {
        if ((std::is_integral<T>::value == true) && (value == 0) ||
            cmp::are_equal (value, 0.0, cmp::epsilon) == true)
                throw std::logic_error("Devision on zero is undefined behaviour");

        for (auto i = 0; i < m_; ++i)
            std::transform (rows_[i], rows_[i] + n_, rows_[i], [value](T arg) { return arg /= value; });

        return *this;
    }

    static Matrix eye(size_t n);
    T determinant () 
    {
        if (n_ != m_)
            throw std::logic_error ("Matrix should be square for computing determinant");
        return determinant_in(); 
    }

private:
    //  Without pivoting and parenthesis for square matrix
    T determinant_in () const
    {
        Matrix<T> tmp = *this;
        auto sign = 1;
        T det;

        if (std::is_integral<T>::value == false)
        {
            //  Begining of Gaussian Elimination

            for (auto i = 0; i < n_ - 1; ++i)
            {
                //  finding out a pivot element
                if (cmp::are_equal(tmp.rows_[i][i], 0.0, cmp::epsilon)) 
                {
                    auto pivot_i = i;

                    for (auto j = i + 1; j < m_; ++j)
                    {
                        if (!cmp::are_equal (tmp.rows_[j][i], 0.0, cmp::epsilon))
                        {
                            pivot_i = j;
                            break;
                        }
                    }
                    if (cmp::are_equal(tmp.rows_[pivot_i][i], 0.0, cmp::epsilon))
                        return 0.0;

                    std::swap (tmp.rows_[i], tmp.rows_[pivot_i]);

                    sign *= (-1);
                }


                for (auto j = i + 1; j < n_; ++j)
                {
                    T koef = tmp.rows_[j][i] / tmp.rows_[i][i];
                    std::transform (tmp.rows_[j] + i, tmp.rows_[j] + n_, tmp.rows_[i] + i, tmp.rows_[j] + i,
                                    [koef] (T first, T second) { return first -= koef * second; });
                }
            }

            det = tmp.rows_[0][0];
            for (auto i = 1; i < n_; ++i)
                det *= tmp.rows_[i][i];
        }
        else 
        {
            //  Begining of Bareiss Algorithm

            for(int i = 0; i < n_ - 1; ++i) 
            {
                //Pivot - row swap needed
                if(tmp.rows_[i][i] == 0) 
                {
                    auto pivot_i = i;

                    for (auto j = i + 1; j < n_; ++j)
                    {
                        if (tmp.rows_[j][i] > tmp.rows_[pivot_i][i])
                        {
                            pivot_i = j;
                            break;
                        }
                    }

                    if (tmp.rows_[pivot_i][i] == T(0))
                        return T(0);

                    std::swap (tmp.rows_[i], tmp.rows_[pivot_i]);

                    sign *= (-1);
                }

                for (int j = i + 1; j < n_; ++j) 
                {
                    for (int k = i + 1; k < n_; ++k) 
                    {                    
                        tmp.rows_[j][k] = tmp.rows_[i][i] * tmp.rows_[j][k] - tmp.rows_[j][i] * tmp.rows_[i][k];
                        if (i != 0)
                            tmp.rows_[j][k] /= tmp.rows_[i-1][i-1];
                    }
                }
            }

            det = tmp.rows_[n_ - 1][n_ - 1];
        }

        det *= sign;
        return det;
    }

};  //  class Matrix


template <typename T>
Matrix<T> operator+ (const Matrix<T>& lhs, const Matrix<T> &rhs)
{
    Matrix<T> tmp{lhs};
    tmp += rhs;
    return tmp;
}

template <typename T>
Matrix<T> operator- (const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    Matrix<T> tmp{lhs};
    tmp -= rhs;
    return tmp;
}

template <typename T>
Matrix<T> operator* (const T value, const Matrix<T> &rhs)
{
    Matrix<T> tmp{rhs};
    return tmp *= value;
}
template <typename T>
Matrix<T> operator* (const Matrix<T> &lhs, const T value) { return value * lhs; }

template <typename T>
Matrix<T> operator/ (const Matrix<T> &lhs, const T value)
{
    Matrix<T> tmp{lhs};
    return tmp /= value;
}

template <typename T>
std::ostream& operator<< (std::ostream& os, const Matrix<T>& lhs)
{
    lhs.dump();
    return os;
}

template <typename T> 
Matrix<T> Matrix<T>::eye (size_t n)
{
    Matrix<T> identity (n, n, T(0));
    for (auto i = 0; i < n; ++i)
        identity.rows_[i][i] = T(1);

    return identity;
}

//  not effective multiplication for common case
template <typename T>
Matrix<T> matrix_product (const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    size_t seq_l = lhs.get_cols();
    if (seq_l != rhs.get_rows())
        throw std::logic_error("Inappropriate size for matrix multiplication");

    size_t new_m_ = lhs.get_rows();
    size_t new_n_ = rhs.get_cols();

    Matrix<T> res (new_m_, new_n_, T(0));
    for (auto i = 0; i < new_m_; ++i)
    {
        for (auto j = 0; j < new_n_; ++j)
        {
            for (auto k = 0; k < seq_l; ++k)
                res[i][j] += lhs[i][k] * rhs[k][j];
        }
    }

    return res;
}

}   //  Matrix_Algebra namespace

#endif
