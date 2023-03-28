#ifndef MY_ARRAY
#define MY_ARRAY

#include <iterator>
#include <iomanip>
#include <memory>
#include <string>
#include <functional>

//  Custom exception structs  //
namespace Custom_Exceptions
{
    struct Print_Except : public std::runtime_error {
        Print_Except(const char *msg) : std::runtime_error(msg) {}
    };

    struct Access_Except : public Print_Except {
        Access_Except() : Print_Except{"The array doesn't have n-th element"} {}
    };

    struct Range_Except : public Print_Except {
        Range_Except() : Print_Except{"This range is not inside into the array"} {}
    };

    struct Top_Except : public Print_Except {
        Top_Except() : Print_Except{"The array is empty for top()"} {}
    };

    struct Pop_Except : public Print_Except {
        Pop_Except() : Print_Except{"The array is empty for pop_back()"} {}
    };

    struct Border_Except : public Print_Except {
        Border_Except() : Print_Except{"Borders of the subarray are should be within the basic array"} {}
    };

    struct Iterator_Except : public Print_Except {
        Iterator_Except() : Print_Except{"For using constructor the 2nd iterator should be reachable from \
                                          the 1st by incrementing the 1st. Distance between 1st and 2nd iterators \
                                          should be less than capacity"} {}
    };
}

namespace My_Array
{

//  Base class Buffer  //
template <typename T>
struct Buffer
{

protected:
    T* buff_ = nullptr;
    size_t size_ = 0, capty_ = 0;

    Buffer (const size_t& capty = 0) :  buff_{(capty == 0) ? nullptr : 
                                                             static_cast<T *>(::operator new (sizeof(T) * capty))},
                                        capty_{capty} {}
    Buffer (const Buffer& rhs) = delete;
    Buffer& operator= (const Buffer &rhs) = delete;

    Buffer (Buffer&& rhs) noexcept : buff_{std::exchange(rhs.buff_, nullptr)},
                                     capty_{std::exchange(rhs.capty_, 0)},
                                     size_ {std::exchange(rhs.size_, 0)} {}

    Buffer& operator= (Buffer&& rhs) noexcept
    {
        std::swap (buff_, rhs.buff_);
        std::swap (capty_, rhs.capty_);
        std::swap (size_, rhs.size_);
        
        return *this;
    }

    ~Buffer () 
    {
        std::destroy(buff_, buff_ + size_);
        ::operator delete(buff_);
    }
};

//  Derived class Array

template <typename T> class Array : private Buffer<T>
{
    using Buffer<T>::buff_;
    using Buffer<T>::capty_;
    using Buffer<T>::size_;

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
    Array (size_t capty, It start, It end): Buffer<T>(capty)
    {
        auto dist = std::distance(start, end);
        if (dist <= 0 || capty < dist)
            throw Custom_Exceptions::Iterator_Except{};

        for (; size_ < dist; ++size_)
            std::construct_at(buff_ + size_, static_cast<T>(*(start + size_)));
    }

    T& operator[] (int n) 
    {
        if (n >= size_)
            throw Custom_Exceptions::Access_Except{};
        return buff_[n];
    }

    const T& operator[] (int n) const
    {
        if (n >= size_)
            throw Custom_Exceptions::Access_Except{};
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
        if (!((begin_i <= size_) && (begin_i < end_i) && (end_i <= capty_)))
            throw Custom_Exceptions::Range_Except{};

        std::destroy (buff_ + begin_i, buff_ + end_i);

        if (end_i < size_)
            for (auto i = begin_i; i < end_i; ++i)
                std::construct_at(static_cast<T*>(&buff_[i]), elem);
        else
            for (; size_ < end_i; ++size_)
                std::construct_at(static_cast<T*>(&buff_[size_]), elem);
    }

    T& top () 
    {
        if (size_ < 1)
            throw Custom_Exceptions::Top_Except{};
        return buff_[size_];
    }

    void pop_back ()
    {
        if (size_ < 1)
            throw Custom_Exceptions::Pop_Except{};
        std::destroy_at (static_cast<T*>(&buff_[--size_]));
    }

    //  Swap part as subarray
    void swap_subarray (size_t first1, size_t last1, size_t first2)
    {
        if (!((first1 < last1) && (last1 <= size_)))
            throw Custom_Exceptions::Border_Except{};

        if (size_ - first2 < last1 - first1)
            throw Custom_Exceptions::Border_Except{};

        for (int i = 0, length = last1 - first1; i < length; ++i)
            std::swap(buff_[first1 + i], buff_[first2 + i]);
    }

    void dump () const
    {
        std::cout << "| ";
        for (auto i = 0; i < size_; i++)
            std::cout << buff_[i] << " | ";
        std::cout << "\n";
    }

    T* begin () const { return buff_; }
    T* end   () const { return buff_ + size_;}

    size_t size     () { return size_;  }
    size_t capacity () { return capty_; }

}; //  class Array

}

#endif