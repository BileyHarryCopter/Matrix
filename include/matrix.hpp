#ifndef MATRIX_LIB
#define MATRIX_LIB

#include <iostream>
#include <iterator>
#include <iomanip>
#include <random>
#include <functional>
#include <concepts>

#include "comparison.hpp"
#include "array.hpp"

//  Custom exception structs  //
namespace Custom_Exceptions
{
    struct Mismatched_Size_Except : public Print_Except {
        Mismatched_Size_Except() : Print_Except{"Size of the matrices should be equal for addition or subtraction"} {}
    };

    struct Division_Overflow_Except : public Print_Except {
        Division_Overflow_Except() : Print_Except{"Division on zero is not allowed"} {}
    };

    struct Determinant_For_Non_Square_Except : public Print_Except {
        Determinant_For_Non_Square_Except() : Print_Except{"The matrix should be square for calling determinant()"} {}
    };

    struct Product_Except : public Print_Except {
        Product_Except() : Print_Except{"Number of rows of the 1st matrix should be equal \
                                        number of coloumns of the 2nd matrix for using matrix multiplication"} {}
    };
}

namespace Matrix_Algebra
{

constexpr int RANDOM_MATRIX_COEF = 100;

template<typename T = double> class Matrix final
{

    size_t n_ = 0, m_ = 0;  
    My_Array::Array<T> container_;

    struct Proxy_Row 
    {
        T *row_ = nullptr;
        T& operator[](size_t n) { return row_[n]; }
        const T& operator[](size_t n) const { return row_[n]; }
    };

    public :

    Matrix (size_t n, size_t m, T value = T{}) : n_{n}, m_{m}, container_{m_ * n_}  
    {
        container_.fill(value);
    }

    //  Constructor of matrix (n x m) from sequence
    template<std::random_access_iterator It> 
    Matrix (size_t n, size_t m, It start, It end) : n_{n}, m_{m}, container_{My_Array::Array<T>(n_ * m_, start, end)} {}

    //  Constructor from nested initializer list
    Matrix (const std::initializer_list<std::initializer_list<T>>& input_list) : m_{input_list.size()}
    {
        n_ = std::max (input_list,  [](std::initializer_list<T> first, std::initializer_list<T> second) 
                                    {return (first.size() <= second.size()) ? 1 : 0;}).size();

        auto input_list_ptr = input_list.begin();

        container_ = My_Array::Array<T>{m_ * n_};

        for (auto i = 0, pivot_i = 0; i < m_; ++i)
        {
            for (auto x : input_list_ptr[i])
                container_.push_back(x);

            auto input_list_i_len = input_list_ptr[i].size();
            if (input_list_i_len < n_)
                for (auto i = input_list_i_len; i < n_; i++)
                    container_.push_back(T(0));

            pivot_i += n_;
        }
    }

    //  Constructor from single initializer list
    Matrix (std::initializer_list<T> input_list) : m_{1}, n_{input_list.size()}, container_{n_}
    {
        for (auto x : input_list)
            container_.push_back(x);
    }

    Proxy_Row       operator[](int m)       { return Proxy_Row {begin_data() + m * n_}; }
    const Proxy_Row operator[](int m) const { return Proxy_Row {begin_data() + m * n_}; }

    size_t get_rows() const { return m_; }
    size_t get_cols() const { return n_; }
    T*   begin_data() const { return container_.begin(); }
    //  Give pointer T* to (*this)[i][j]
    T*  get_address(int i, int j) const { return container_.begin() + i * n_ + j; }

    T& trace() const 
    {
        T trace_ = 0;
        for (auto i = 0; (i < n_) && (i < m_); ++i)
            trace_ += *this[i][i];
        return trace_;
    }

    void dump (std::ostream& os = std::cout) const
    {
        for (auto i = 0; i < m_; ++i)
        {
            os << '\t';
            for (auto j = 0; j < n_; ++j)
                os << std::setw(12) << std::right << std::fixed << container_[i * n_ + j] << " ";
            os << "\n";
        }
    }

    bool operator==(const Matrix& rhs) const
    {
        if (this == &rhs)
            return true;

        if (m_ != rhs.m_ || n_ != rhs.n_)
            return false;

        if constexpr (std::is_integral_v<T>)
            return std::equal(container_.begin(), container_.end(), rhs.container_.begin());

        if constexpr (std::is_floating_point_v<T>)
            return std::equal(container_.begin(), container_.end(), rhs.container_.begin(),
                              [](T first, T second) { return cmp::are_equal(first, second, cmp::epsilon); });
        
    }
    bool operator!=(const Matrix &rhs) const { return !(*this == rhs); }

    Matrix operator-()
    {
        Matrix tmp = *this;
        std::transform (container_.begin(),     container_.end(), 
                        tmp.container_.begin(), std::negate());

        return tmp;
    }
    Matrix& inverse_sign()
    {
        std::transform (container_.begin(), container_.end(), 
                        container_.begin(), std::negate());

        return *this;
    }

    Matrix& operator+=(const Matrix& rhs)
    {
        if (m_ != rhs.m_ || n_ != rhs.n_)
            throw Custom_Exceptions::Mismatched_Size_Except{};

        std::transform (container_.begin(),     container_.end(), 
                        rhs.container_.begin(), container_.begin(), std::plus());

        return *this;
    }

    Matrix& operator-=(const Matrix& rhs)
    {
        if (m_ != rhs.m_ || n_ != rhs.n_)
            throw Custom_Exceptions::Mismatched_Size_Except{};

        std::transform (container_.begin(),     container_.end(), 
                        rhs.container_.begin(), container_.begin(), std::minus());

        return *this;
    }

    Matrix & operator*=(T value)
    {
        std::transform (container_.begin(), container_.end(), 
                        container_.begin(), [value](T arg) { return arg *= value; });

        return *this;
    }

    Matrix& operator/=(T value)
    {
        if ((std::is_integral<T>::value == true) && (value == 0) ||
            cmp::are_equal (value, 0.0, cmp::epsilon) == true)
                throw Custom_Exceptions::Division_Overflow_Except{};

        std::transform (container_.begin(), container_.end(), 
                        container_.begin(), [value](T arg) { return arg /= value; });

        return *this;
    }

    static Matrix eye   (size_t n);
    static Matrix random(size_t n, const T &det);

    T determinant() requires std::integral<T>
    {
        if (n_ != m_)
            throw Custom_Exceptions::Determinant_For_Non_Square_Except{};

        Matrix<T> tmp = *this;
        T sign = T(1);

        for(auto i = 0; i < n_ - 1; ++i) 
        {
            //  Finding a pivot element
            if(tmp[i][i] == 0) 
            {
                auto pivot_i = i;

                for (auto j = i + 1; j < m_; ++j)
                {
                    if (std::abs(tmp[j][i]) > std::abs(tmp[pivot_i][i]))
                    {
                        pivot_i = j;
                        break;
                    }
                }
                if (tmp[pivot_i][i] == T(0))
                    return T(0);

                tmp.container_.swap_subarray (i * n_, (i + 1) * n_, pivot_i * n_);

                sign *= T(-1);
            }
            // The under is faster than previous version:
            // for (auto j = i + 1; j < m_; ++j)
            // {
            //     for (int k = i + 1; k < n_; ++k)
            //     {
            //         tmp[j][k] = tmp[i][i] * tmp[j][k] - tmp[j][i] * tmp[i][k];
            //         if (i != 0)
            //             tmp[j][k] /= tmp[i - 1][i - 1];
            //     }
            // }

            if (i == 0)
            {
                for (auto j = 1; j < m_; ++j)
                {
                    auto pivot_i = tmp[0][0], pivot_j = tmp[j][0];

                    std::transform (tmp.get_address(j, 1), tmp.get_address (j + 1, 0), tmp.get_address (0, 1), tmp.get_address (j, 1),
                                    [pivot_i, pivot_j] (T first, T second) { return first = first * pivot_i - second * pivot_j; });
                }
                continue;
            }

            for (auto j = i + 1; j < m_; ++j) 
            {
                auto pivot_i = tmp[i][i], pivot_j = tmp[j][i], koef = tmp[i - 1][i - 1];

                std::transform (tmp.get_address(j, i + 1), tmp.get_address (j + 1, 0), tmp.get_address (i, i + 1), tmp.get_address (j, i + 1),
                                [pivot_i, pivot_j, koef] (T first, T second) { return first = (first * pivot_i - second * pivot_j) / koef; });
            }
        }

        auto det = tmp[n_ - 1][n_ - 1];

        return det * sign;
    }

    T determinant() requires std::floating_point<T>
    {
        if (n_ != m_)
            throw Custom_Exceptions::Determinant_For_Non_Square_Except{};

        Matrix <T> tmp = *this;
        T sign = T(1);

        for (auto i = 0; i < n_ - 1; ++i)
        {
            //  Finding a pivot element
            if (cmp::are_equal(tmp[i][i], 0.0, cmp::epsilon))
            {
                auto pivot_i = i;

                for (auto j = i + 1; j < m_; ++j)
                {
                    if (!cmp::are_equal(tmp[j][i], 0.0, cmp::epsilon))
                    {
                        pivot_i = j;
                        break;
                    }
                }
                if (cmp::are_equal(tmp[pivot_i][i], 0.0, cmp::epsilon))
                    return T(0);

                tmp.container_.swap_subarray(i * n_, (i + 1) * n_, pivot_i * n_);

                sign *= T(-1);
            }

            for (auto j = i + 1; j < m_; ++j)
            {
                auto koef = tmp[j][i] / tmp[i][i];

                std::transform (tmp.get_address(j, i), tmp.get_address(j + 1, 0),
                                tmp.get_address(i, i), tmp.get_address(j, i),
                                [koef] (T first, T second) { return first -= koef * second; });
            }
        }

        T det = tmp[0][0];
        for (auto i = 1; i < n_; ++i)
            det *= tmp[i][i];

        return det * sign;
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
        identity[i][i] = T(1);

    return identity;
}

template <typename T>
Matrix<T> Matrix<T>::random (size_t n, const T& det)
{
    Matrix<T> tmp = Matrix<T>::eye(n);
    tmp[n - 1][n - 1] = det;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, RANDOM_MATRIX_COEF);

    for (auto i = 1; i < n; ++i)
    {
        auto koef = dist(gen);

        std::transform (tmp.container_.begin(), tmp.container_.begin() + n,
                        tmp.container_.begin() + n * i, tmp.container_.begin(), 
                        [koef](T first, T second) { return first += second * koef; });
    }

    for (auto i = 1; i < n; ++i)
    {
        auto koef = dist(gen);

        std::transform (tmp.container_.begin() + n * i, tmp.container_.begin() + n * (i + 1),
                        tmp.container_.begin(), tmp.container_.begin() + n * i, 
                        [koef](T first, T second) { return first += second * koef; });

    }

    return tmp;
}

//  not effective multiplication for general case
template <typename T>
Matrix<T> matrix_product (const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    size_t seq_l = lhs.get_cols();
    if (seq_l != rhs.get_rows())
        throw Custom_Exceptions::Product_Except{};

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
