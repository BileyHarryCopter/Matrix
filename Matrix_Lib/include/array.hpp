#ifndef MY_ARRAY
#define MY_ARRAY

#include <iterator>
#include <iomanip>
#include <functional>
#include <memory>
// #include <type_traits>

namespace My_Array
{

template <typename T> struct Buffer
{

protected:
    T* buff_ = nullptr;
    size_t capty_ = 0;
    size_t size_ = 0;

    Buffer (const size_t& capty = 0) :  buff_{(capty == 0) ? nullptr : 
                                                             static_cast<T *>(::operator new (sizeof(T) * capty))},
                                        capty_{capty} {}
    Buffer (const Buffer& rhs) = delete;
    Buffer& operator= (const Buffer &rhs) = delete;

    Buffer (Buffer&& rhs) noexcept : buff_{std::exchange(rhs.buff_, nullptr)},
                                     size_{std::exchange(rhs.size_, 0)},
                                     capty_{std::exchange(rhs.capty_, 0)} {}

    Buffer& operator= (Buffer&& rhs) noexcept
    {
        std::swap (buff_, rhs.buff_);
        std::swap (size_, rhs.size_);
        std::swap (capty_, rhs.capty_);
        return *this;
    }

    ~Buffer ()
    {
        std::destroy(buff_, buff_ + size_);
        ::operator delete (buff_);
    }
};

template <typename T> class Array : private Buffer<T>
{
    using Buffer<T>::buff_;
    using Buffer<T>::size_;
    using Buffer<T>::capty_;

public:

    Array (const size_t& capty = 0) : Buffer<T>(capty) {}
    Array (Array&& rhs) = default;
    Array& operator=(Array &&rhs) = default;

    Array (const Array& rhs) : Buffer<T>(rhs.capty_)
    {
        for (; size_ < rhs.size_; ++size_)
            std::construct_at (buff_ + size_, T(rhs.buff_[size_]));
    }

    Array& operator= (const Array& rhs)
    {
        Array tmp(rhs);
        std::swap (*this, tmp);
        return *this;
    }

    template<std::input_iterator It>
    Array (size_t capty, const It start, const It end) : Buffer<T>(capty) {size_ = end - start; std::copy(start, end, buff_);}

    T& operator[] (int n) 
    {
        if (n > size_)
            throw std::runtime_error ("The array doesn't have n element");
        return buff_[n]; 
    }

    const T& operator[] (int n) const
    {
        if (n > size_)
            throw std::runtime_error ("The array doesn't have n element");
        return buff_[n];
    }

    void push_back (const T& elem) 
    {
        if (size_ == capty_)
        {
            Array tmp (2 * capty_ + 1);
            for (; tmp.size_ < size_; ++tmp.size_)
                std::construct_at (tmp.buff_ + tmp.size_, buff_[tmp.size_]);
            std::swap (*this, tmp);
        }
        std::construct_at (buff_ + size_, elem);
        size_++;
    }

    void fill (const T& elem)
    {
        std::destroy (buff_, buff_ + size_);
        for (size_ = 0; size_ < capty_; ++size_)
            std::construct_at (std::addressof(buff_[size_]), elem);
    }

    void fill (size_t begin_i, size_t end_i, const T& elem)
    {
        if (!((begin_i < size_) && (begin_i < end_i) && (end_i <= capty_)))
            throw std::runtime_error ("This range is not inside into the array");

        std::destroy (buff_ + begin_i, buff_ + end_i);

        if (end_i < size_)
            for (auto i = begin_i; i < end_i; ++i)
                std::construct_at(std::addressof(buff_[i]), elem);
        else
            for (; size_ < end_i; ++size_)
                std::construct_at(std::addressof(buff_[size_]), elem);
    }

    T& top () 
    {
        if (size_ < 1)
            throw std::runtime_error ("top() from empty array is illegal");
        return buff_[size_];
    }

    void pop_back ()
    {
        if (size_ < 1)
            throw std::runtime_error ("The array is empty for pop_back()");
        std::destroy_at (buff_[--size_]);
    }

    //  Swap part as subarray
    void swap_subarray (size_t first1, size_t last1, size_t first2)
    {
        if (!((first1 < last1) && (last1 <= size_)))
            throw std::runtime_error ("Borders of the 1st subarray are should be within the main array");

        if (size_ - first2 < last1 - first1)
            throw std::runtime_error ("Borders of the 2nd subarray are should be within the main array");

        for (auto i = first1; i < last1; ++i)
            std::swap (buff_[i], buff_[first2 + i]);
    }

    void dump () 
    {
        std::cout << "| ";
        for (auto i = 0; i < size_; i++)
            std::cout << buff_[i] << " | ";
        std::cout << "\n";
    }

    T* begin () const { return buff_; }
    T* end   () const { return buff_ + size_ + 1;}

    size_t size     () { return size_;  }
    size_t capacity () { return capty_; }

}; //  class Array

}

#endif