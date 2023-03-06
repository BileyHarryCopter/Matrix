#ifndef MY_ARRAY
#define MY_ARRAY

#include <iterator>
#include <iomanip>
#include <memory>
#include <string>
#include <functional>

namespace My_Array
{

//  Custom exception structs  //

struct Print_Except : public std::runtime_error
{
    Print_Except(const char *msg) : std::runtime_error(msg) {}
};

struct Access_Except : public Print_Except
{
    Access_Except() : Print_Except{"The array doesn't have n-th element"} {}
};

struct Range_Except : public Print_Except
{
    Range_Except() : Print_Except{"This range is not inside into the array"} {}
};

struct Top_Except : public Print_Except
{
    Top_Except() : Print_Except{"The array is empty for top()"} {}
};

struct Pop_Except : public Print_Except
{
    Pop_Except() : Print_Except{"The array is empty for pop_back()"} {}
};

struct Border_Except : public Print_Except
{
    Border_Except() : Print_Except{"Borders of the subarray are should be within the basic array"} {}
};

struct Iterator_Except : public Print_Except
{
    Iterator_Except() : Print_Except{"For using constructor the 2nd it should be reachable from \
                                      the 1st it by incrementing the 1st"} {}
};

//  Base class Buffer  //

template <typename T>
struct Buffer
{

protected:
    T* buff_ = nullptr;
    size_t capty_ = 0;

    Buffer (const size_t& capty = 0) :  buff_{(capty == 0) ? nullptr : 
                                                             static_cast<T *>(::operator new (sizeof(T) * capty))},
                                        capty_{capty} {}
    Buffer (const Buffer& rhs) = delete;
    Buffer& operator= (const Buffer &rhs) = delete;

    Buffer (Buffer&& rhs) noexcept : buff_{std::exchange(rhs.buff_, nullptr)},
                                     capty_{std::exchange(rhs.capty_, 0)} {}

    Buffer& operator= (Buffer&& rhs) noexcept
    {
        std::swap (buff_, rhs.buff_);
        std::swap (capty_, rhs.capty_);
        return *this;
    }

    ~Buffer () {::operator delete (buff_);}
};

//  Derived class Array

template <typename T> class Array : private Buffer<T>
{
    using Buffer<T>::buff_;
    using Buffer<T>::capty_;
    size_t size_ = 0;

public:

    Array (const size_t& capty = 0) : Buffer<T>(capty) {}
    Array (Array&& rhs) = default;
    Array& operator=(Array &&rhs) = default;

    Array (const Array& rhs) : Buffer<T>(rhs.capty_)
    {
        for (; size_ < rhs.size_; ++size_)
            std::construct_at (buff_ + size_, T(rhs.buff_[size_]));
    }

    ~Array() { std::destroy(buff_, buff_ + size_); }

    Array& operator= (const Array& rhs)
    {
        Array tmp(rhs);
        std::swap (*this, tmp);
        return *this;
    }

    template<std::input_iterator It>
    Array (size_t capty, It start, It end): Buffer<T>(capty)
    {
        auto dist = std::distance(start, end);
        if (dist < 0)
            throw Iterator_Except{};

        size_ = dist;
        std::copy (start, end, buff_);
    }

    T& operator[] (int n) 
    {
        if (n > size_)
            throw Access_Except{};
        return buff_[n];
    }

    const T& operator[] (int n) const
    {
        if (n > size_)
            throw Access_Except{};
        return buff_[n];
    }

    void clear () 
    {
        std::destroy (buff_, buff_ + size_);
        size_ = 0;
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
        clear();
        for (auto i = 0; i < capty_; ++i)
            push_back(elem);
    }
    
    void fill (size_t begin_i, size_t end_i, const T& elem)
    {
        if (!((begin_i < size_) && (begin_i < end_i) && (end_i <= capty_)))
            throw Range_Except{};

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
            throw Top_Except{};
        return buff_[size_];
    }

    void pop_back ()
    {
        if (size_ < 1)
            throw Pop_Except{};
        std::destroy_at (buff_[--size_]);
    }

    //  Swap part as subarray
    void swap_subarray (size_t first1, size_t last1, size_t first2)
    {
        if (!((first1 < last1) && (last1 <= size_)))
            throw Border_Except{};

        if (size_ - first2 < last1 - first1)
            throw Border_Except{};

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