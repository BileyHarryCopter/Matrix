#ifndef MATRIX_LIB
#define MATRIX_LIB

#include <iostream>
#include <iterator>
#include <iomanip>
#include <type_traits>
#include <functional>
#include <random>

#include "comparison.hpp"
#include "array.hpp"

namespace Matrix_Algebra
{

using namespace My_Array;

// Custom exception structs

struct Addition_Except : public Print_Except
{
    Addition_Except() : Print_Except{"Size of the matrices should be equal for addition"} {}
};

struct Subtraction_Except : public Print_Except
{
    Subtraction_Except() : Print_Except{"Size of the matrices should be equal for subtraction"} {}
};

struct Division_Overflow_Except : public Print_Except
{
    Division_Overflow_Except() : Print_Except{"Division on zero is not allowed"} {}
};

struct Determinant_For_Non_Square_Except : public Print_Except
{
    Determinant_For_Non_Square_Except() : Print_Except{"The matrix should be square for calling determinant()"} {}
};

struct Product_Except : public Print_Except
{
    Product_Except() : Print_Except{"Number of rows of the 1st matrix should be equal \
                                     number of coloumns of the 2nd matrix for using matrix multiplication"} {}
};

template<typename T = double> class Matrix
{

    size_t n_ = 0, m_ = 0;
    Array<T> container_;

    struct Proxy_Row 
    {
        T *row_ = nullptr;
        T& operator[](size_t n) { return row_[n]; }
        const T& operator[](size_t n) const { return row_[n]; }
    };

    public :

    Matrix (size_t n, size_t m, T value = T{}) : n_{n}, m_{m}, container_{m_ * n_}  { container_.fill(value); }

    //  Constructor of matrix (n x m) from sequence
    template<std::input_iterator It> 
    Matrix (size_t n, size_t m, It start, It end) : n_{n}, m_{m}, container_{Array<T>(n_ * m_, start, end)} {}

    //  Constructor from nested initializer list
    Matrix (const std::initializer_list<std::initializer_list<T>>& input_list) : m_{input_list.size()}
    {
        n_ = std::max (input_list,  [](std::initializer_list<T> first, std::initializer_list<T> second) 
                                    {return (first.size() <= second.size()) ? second.size() : first.size();}).size();
        auto input_list_ptr = input_list.begin();

        container_ = Array<T>{m_ * n_};

        for (auto i = 0, pivot_i = 0; i < m_; ++i)
        {
            for (auto x : input_list_ptr[i])
                container_.push_back(x);

            auto input_list_i_len = input_list_ptr[i].size();
            if (input_list_i_len < n_)
                container_.fill(pivot_i + input_list_i_len, pivot_i + n_, T(0));

            pivot_i += n_;
        }
    }

    //  Constructor from single initializer list
    Matrix (std::initializer_list<T> input_list) : m_{1}, n_{input_list.size()}, container_{n_}
    {
        for (auto x : input_list)
            container_.push_back(x);
    }

    Proxy_Row operator[](int m)             { return Proxy_Row {begin_data() + m * n_}; }
    const Proxy_Row operator[](int m) const { return Proxy_Row {begin_data() + m * n_}; }

    size_t get_rows() const { return m_; }
    size_t get_cols() const { return n_; }
    T*  begin_data () const { return container_.begin(); }

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
                os << "|" << std::setw(5) << std::left << container_[i * n_ + j];
            os << "|\n";
        }
    }

    bool operator== (const Matrix& rhs) const
    {
        if (this == &rhs)
            return true;

        if (m_ != rhs.m_ || n_ != rhs.n_)
            return false;

        if ((std::is_integral<T>::value == true) &&
            (!std::equal(container_.begin(), container_.begin() + m_ * n_, rhs.container_.begin())))
                return false;
        else if ((std::is_integral<T>::value == false) &&
                 (!std::equal(container_.begin(), container_.begin() + m_ * n_, rhs.container_.begin(),
                            [](T first, T second)
                            { return cmp::are_equal(first, second, cmp::epsilon); })))
                return false;

        return true;
    }
    bool operator!=(const Matrix &rhs) const { return !(*this == rhs); }

    Matrix operator- ()
    {
        Matrix tmp = *this;
        std::transform (container_.begin(), container_.begin() + m_ * n_, 
                        tmp.container_.begin(), std::negate());

        return tmp;
    }
    Matrix& inverse_sign ()
    {
        std::transform (container_.begin(), container_.begin() + m_ * n_, 
                        container_.begin(), std::negate());

        return *this;
    }

    Matrix& operator+= (const Matrix& rhs)
    {
        if (m_ != rhs.m_ || n_ != rhs.n_)
            throw Addition_Except{};

        std::transform (container_.begin(), container_.begin() + m_ * n_, 
                        rhs.container_.begin(), container_.begin(), std::plus());

        return *this;
    }

    Matrix& operator-= (const Matrix& rhs)
    {
        if (m_ != rhs.m_ || n_ != rhs.n_)
            throw Subtraction_Except{};

        std::transform (container_.begin(),     container_.begin() + m_ * n_, 
                        rhs.container_.begin(), container_.begin(), std::minus());

        return *this;
    }

    Matrix & operator*=(T value)
    {
        std::transform (container_.begin(), container_.begin() + m_ * n_, 
                        container_.begin(), [value](T arg) { return arg *= value; });

        return *this;
    }

    Matrix& operator/= (T value)
    {
        if ((std::is_integral<T>::value == true) && (value == 0) ||
            cmp::are_equal (value, 0.0, cmp::epsilon) == true)
                throw Division_Overflow_Except{};

        std::transform (container_.begin(), container_.begin() + m_ * n_, 
                        container_.begin(), [value](T arg) { return arg /= value; });

        return *this;
    }

    static Matrix eye (size_t n);
    static Matrix random (size_t n, const T &det);

    T determinant() requires std::is_integral<T>::value
    {
        if (n_ != m_)
            throw Determinant_For_Non_Square_Except{};

        Matrix<T> tmp = *this;
        auto sign = 1;

        //  case if i == 0
        for (int j = 1; j < m_; ++j)
        {
            auto pivot_i = tmp[0][0], pivot_j = tmp[j][0];
            std::transform (tmp.begin_data() + n_ * j + 1, tmp.begin_data() + n_ * (j + 1),
                            tmp.begin_data() + 1, tmp.begin_data() + n_ * j + 1,
                            [pivot_i, pivot_j] (T first, T second) { return first = first * pivot_i - second * pivot_j;});
        }

        for(int i = 1; i < n_ - 1; ++i) 
        {
            //Pivot - row swap needed
            if(tmp[i][i] == 0) 
            {
                auto pivot_i = i;

                for (auto j = i + 1; j < n_; ++j)
                {
                    if (tmp[j][i] > tmp[pivot_i][i])
                    {
                        pivot_i = j;
                        break;
                    }
                }
                if (tmp[pivot_i][i] == T(0))
                    return T(0);

                tmp.container_.swap_subarray (i * n_, (i + 1) * n_, pivot_i * n_);

                sign *= (-1);
            }

            //  The under is more fast than previous version:
            //  for (int k = i + 1; k < n_; ++k) 
            //  {                    
            //      tmp[j][k] = tmp[i][i] * tmp[j][k] - tmp[j][i] * tmp[i][k];
            //      if (i != 0)
            //          tmp[j][k] /= tmp[i-1][i-1];
            //  }

            for (int j = i + 1; j < m_; ++j) 
            {
                auto pivot_i = tmp[i][i], pivot_j = tmp[j][i];
                std::transform(tmp.begin_data() + n_ * j + (i + 1), tmp.begin_data() + n_ * (j + 1),
                               tmp.begin_data() + n_ * i + (i + 1), tmp.begin_data() + n_ * j + (i + 1),
                               [pivot_i, pivot_j] (T first, T second) { return first = first * pivot_i - second * pivot_j; });
            }
        }

        T det = tmp[n_ - 1][n_ - 1];
    
        return det * sign;
    }

    T determinant() requires std::is_floating_point<T>::value
    {
        if (n_ != m_)
            throw Determinant_For_Non_Square_Except{};

        Matrix <T> tmp = *this;
        auto sign = 1;

        for (auto i = 0; i < n_ - 1; ++i)
        {
            //  finding out a pivot element
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

                tmp.container_.swap_subarray (i * n_, (i + 1) * n_, pivot_i * n_);

                sign *= (-1);
            }

            for (auto j = i + 1; j < m_; ++j)
            {
                T koef = tmp[j][i] / tmp[i][i];
                std::transform (tmp.begin_data() + j * n_ + i, tmp.begin_data() + j * n_ + n_,
                                tmp.begin_data() + i * n_ + i, tmp.begin_data() + j * n_ + i,
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
    std::uniform_int_distribution<> dist(1, 100);

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

//  not effective multiplication for common case
template <typename T>
Matrix<T> matrix_product (const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    size_t seq_l = lhs.get_cols();
    if (seq_l != rhs.get_rows())
        throw Product_Except{};

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
